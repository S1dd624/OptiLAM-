import os
import io
import math
import cairosvg
from PIL import Image
from lxml import etree
import re

def normalize_svg_canvas(svg_path):
    with open(svg_path, "r") as f:
        svg_content = f.read()

    parser = etree.XMLParser(remove_blank_text=True)
    root = etree.fromstring(svg_content.encode(), parser)

    viewBox = root.get("viewBox")
    if viewBox:
        x_min, y_min, width, height = map(float, viewBox.split())
    else:
        width = float(root.get("width", 100))
        height = float(root.get("height", 100))
        x_min, y_min = 0, 0
        root.set("viewBox", f"{x_min} {y_min} {width} {height}")

    max_dim = max(width, height)

    if width < max_dim:
        x_min -= (max_dim - width) / 2
    if height < max_dim:
        y_min -= (max_dim - height) / 2

    root.set("viewBox", f"{x_min} {y_min} {max_dim} {max_dim}")
    root.set("width", f"{max_dim}")
    root.set("height", f"{max_dim}")

    return root, x_min, y_min, max_dim

def lines_to_path(root):
    NS = "http://www.w3.org/2000/svg"
    line_elements = list(root.iter(f"{{{NS}}}line"))
    print(f"  [DEBUG] Converting {len(line_elements)} lines to path...")

    points = []

    for line in line_elements:
        try:
            x1, y1 = float(line.get("x1")), float(line.get("y1"))
            x2, y2 = float(line.get("x2")), float(line.get("y2"))
        except (TypeError, ValueError):
            continue
        points.append((x1, y1))
        points.append((x2, y2))
        parent = line.getparent()
        parent.remove(line)

    if not points:
        print("  [WARNING] No points found from lines.")
        return False

    path_data = "M " + " L ".join(f"{x},{y}" for x, y in points) + " Z"
    path_element = etree.Element(f"{{{NS}}}path")
    path_element.set("d", path_data)
    path_element.set("fill", "black")
    path_element.set("stroke", "black")

    g_elem = root.find(f"{{{NS}}}g")
    if g_elem is not None:
        g_elem.append(path_element)
    else:
        root.append(path_element)

    print(f"  [SUCCESS] Created namespaced path from {len(points)} points.")
    return True



def rotate_and_convert_to_bmp(svg_path, output_svgs_folder, output_bmps_folder, angle_step=30):
    os.makedirs(output_svgs_folder, exist_ok=True)
    os.makedirs(output_bmps_folder, exist_ok=True)

    match = re.search(r"_(\d+)\.svg$", os.path.basename(svg_path))
    if not match:
        print(f"Skipping file {svg_path}: Could not extract layer number.")
        return
    LLL = match.group(1).zfill(3)

    root, x_min, y_min, max_dim = normalize_svg_canvas(svg_path)
    success = lines_to_path(root)
    paths = list(root.iter("{http://www.w3.org/2000/svg}path"))
    if not success or not paths:
        print(f"Skipping file {svg_path}: No paths found after line conversion.")
        return


    normalized_svg_path = os.path.join(output_svgs_folder, f"{LLL}.svg")
    with open(normalized_svg_path, "wb") as f:
        f.write(etree.tostring(root, pretty_print=True))
    print(f"Saved normalized SVG: {normalized_svg_path}")

    bg_rect = etree.Element(
        "rect", x=str(x_min), y=str(y_min), width=str(max_dim), height=str(max_dim), fill="white"
    )
    root.insert(0, bg_rect)
    cx, cy = x_min + (max_dim / 2), y_min + (max_dim / 2)

    for angle in range(0, 360, angle_step):
        root_copy = etree.fromstring(etree.tostring(root))
        paths_copy = list(root_copy.iter("{http://www.w3.org/2000/svg}path"))

        for path in paths_copy:
            path.set("transform", f"rotate({angle}, {cx}, {cy})")
            style = path.get("style", "")
            if "fill:none" in style:
                style = style.replace("fill:none", "fill:black")
            elif "fill:" not in style:
                style += ";fill:black" if style else "fill:black"
            path.set("style", style.strip())

        svg_bytes = etree.tostring(root_copy, pretty_print=True)
        try:
            png_data = cairosvg.svg2png(
                bytestring=svg_bytes, output_width=int(max_dim), output_height=int(max_dim)
            )
        except Exception as e:
            print(f"Error converting SVG to PNG at angle {angle}: {e}")
            continue

        img = Image.open(io.BytesIO(png_data)).convert("L")
        bw = img.point(lambda p: 0 if p < 200 else 255, "1")

        bmp_filename = f"{LLL}_00_{angle:03d}.bmp"
        bmp_path = os.path.join(output_bmps_folder, bmp_filename)
        bw.save(bmp_path, "BMP")
        print(f"Saved BMP: {bmp_path}")

def batch_process_svgs(input_folder, output_svgs_folder, output_bmps_folder, angle_step=30):
    os.makedirs(output_svgs_folder, exist_ok=True)
    os.makedirs(output_bmps_folder, exist_ok=True)

    svg_files = [f for f in sorted(os.listdir(input_folder)) if f.endswith(".svg")]
    if not svg_files:
        print(f"No SVG files found in {input_folder}")
        return

    print(f"Found {len(svg_files)} SVG files to process:")
    for filename in svg_files:
        svg_path = os.path.join(input_folder, filename)
        print(f"\n{'='*60}\nProcessing: {filename}\n{'='*60}")
        try:
            rotate_and_convert_to_bmp(svg_path, output_svgs_folder, output_bmps_folder, angle_step)
        except Exception as e:
            print(f"Error processing {filename}: {e}")
            import traceback
            traceback.print_exc()

if __name__ == "__main__":
    batch_process_svgs(
        r"C:\Users\iitsi\Nesting_GA\Nesting_GA\Files\final_generator\PRISM SLICES HATCHED",
        r"C:\Users\iitsi\Nesting_GA\Nesting_GA\Files\final_generator\osvgs",
        r"C:\Users\iitsi\Nesting_GA\Nesting_GA\Files\final_generator\obmps",
        angle_step=45
    )
