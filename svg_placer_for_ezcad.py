import os
from lxml import etree


def process_svg(svg_path, rotation_angle):
    """Rotates an SVG and returns the modified SVG element (without bounding box)."""

    with open(svg_path, "r") as f:
        svg_content = f.read()
    parser = etree.XMLParser(remove_blank_text=True)
    svg_layer = etree.fromstring(svg_content.encode(), parser)

    viewBox = svg_layer.get("viewBox")
    if not viewBox:
        print(f"Skipping {svg_path} - No viewBox found.")
        return None

    x_min, y_min, width, height = map(float, viewBox.split())
    cx, cy = x_min + width / 2, y_min + height / 2

    # Create a group for the rotated content
    g_rotated = etree.Element("g", transform=f"rotate({rotation_angle}, {cx}, {cy})")

    # Move original SVG elements into the rotated group
    for element in svg_layer:
        g_rotated.append(element)

    return g_rotated, x_min, y_min


def assemble_svg(
    x_positions,
    y_positions,
    rotations,
    processed_svg_folder,
    output_svg_path,
    canvas_height_mm,
):
    """Places modified SVGs onto a larger canvas with transformations."""

    max_x = max(x_positions) * 4
    canvas_width_mm = max_x + 200  # Set width to max X coordinate

    svg_ns = "http://www.w3.org/2000/svg"
    root = etree.Element(
        "svg",
        xmlns=svg_ns,
        width=f"{canvas_width_mm}mm",
        height=f"{canvas_height_mm}mm",
        viewBox=f"0 0 {canvas_width_mm} {canvas_height_mm}",
    )
    print(len(rotations))
    for index in range(len(rotations)):
        x, y, theta = x_positions[index] * 4, y_positions[index] * 4, rotations[index]
        LLL = f"{index+1:03d}"  # Format filename as 001, 002, etc.

        svg_path = os.path.join(processed_svg_folder, f"{LLL}.svg")
        print("processing ", index, x, y, theta, svg_path)
        if not os.path.exists(svg_path):
            print(f"Skipping {LLL}.svg - File not found.")
            continue

        modified_svg, x_min, y_min = process_svg(svg_path, theta)
        print(x_min, y_min)
        if modified_svg is None:
            continue

        # Wrap in a translation group for final placement
        g_outer = etree.Element("g", transform=f"translate({x - x_min}, {y - y_min})")
        g_outer.append(modified_svg)
        print("appended modified svg")
        root.append(g_outer)

    with open(output_svg_path, "wb") as f:
        f.write(etree.tostring(root, pretty_print=True))

    print(f"Final assembled SVG saved as: {output_svg_path}")


# Example usage:

# below are x,y,r for run 70 on HL ran 12 hr on Jun 20th results obtained on Jun 21st

rotations = [90, 270, 90, 150, 240, 270, 270, 90, 90, 30, 90, 30, 120, 30, 150, 330, 120, 180, 210, 300, 150, 210, 120, 60, 150, 90, 270, 210, 240, 30]
x_positions = [53, 2, 5, 80, 64, 37, 82, 28, 58, 63, 19, 21, 25, 14, 51, 63, 36, 1, 76, 0, 57, 36, 81, 46, 60, 0, 21, 80, 35, 74]
y_positions = [70, 4, 19, 77, 84, 64, 55, 37, 50, 35, 76, 60, 21, 44, 18, 19, 83, 37, 41, 70, 2, 10, 1, 33, 65, 53, 0, 20, 49, 62]

processed_svg_folder = "C:\\Users\\iitsi\\Nesting_GA\\Nesting_GA\\Files\\final_generator\\otptunhatchedsvgsjun20"
output_svg_path = "C:\\Users\\iitsi\\Nesting_GA\\Nesting_GA\\Files\\final_generator\\j21opunhatch.svg"
canvas_height_mm = 400

assemble_svg(
    x_positions,
    y_positions,
    rotations,
    processed_svg_folder,
    output_svg_path,
    canvas_height_mm,
)
