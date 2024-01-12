def file_to_cpp_header(image_path, csv_path, output_path, image_array_name, csv_array_name):
    # Read and convert the image file
    with open(image_path, 'rb') as image_file:
        image_content = image_file.read()
    image_hex_array = [f"0x{hex(byte)[2:].zfill(2)}" for byte in image_content]

    # Read the CSV file
    with open(csv_path, 'r') as csv_file:
        csv_content = csv_file.readlines()

    # Write to the output header file
    with open(output_path, 'w') as header_file:
        # Writing the image data
        header_file.write(f"unsigned char {image_array_name}[] = {{\n")
        header_file.write(', '.join(image_hex_array))
        header_file.write("\n};\n\n")

        # Writing the CSV data
        header_file.write(f"const char* {csv_array_name}[] = {{\n")
        for line in csv_content:
            header_file.write(f'"{line.strip()}",\n')
        header_file.write("nullptr};\n")  # nullptr to mark the end of the array

# Example usage
file_to_cpp_header("texture_atlas.png", 
                   "texture_positions.csv", 
                   "atlas.h", 
                   "textureAtlasData", 
                   "texturePositionsData")

print("Generated texture header")