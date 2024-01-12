import os
import csv
from PIL import Image, ImageOps
import math

def strip_extension(filename):
    return os.path.splitext(filename)[0]

def calculate_average_color(image):
    # Calculate the average color of an image
    color_tuple = [0, 0, 0]
    for channel in range(3):
        pixels = image.getdata(band=channel)
        values = [pixel for pixel in pixels]
        color_tuple[channel] = sum(values) // len(values)
    return tuple(color_tuple)

def stitch_textures(directory, texture_size=(16, 16), padding= 3, output_filename='texture_atlas.png', csv_filename='texture_positions.csv'):
    image_files = [f for f in os.listdir(directory) if f.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp'))]

    num_images = len(image_files)
    padded_size = (texture_size[0] + padding * 2, texture_size[1] + padding * 2)

    atlas_cols = int(math.ceil(math.sqrt(num_images)))
    atlas_rows = int(math.ceil(num_images / atlas_cols))
    atlas_size = (padded_size[0] * atlas_cols, padded_size[1] * atlas_rows)

    atlas = Image.new('RGBA', atlas_size)

    with open(csv_filename, mode='w', newline='') as file:
        writer = csv.writer(file)

        for i, image_file in enumerate(image_files):
            with Image.open(os.path.join(directory, image_file)) as img:

                centered_img = Image.new('RGBA', texture_size, (0, 0, 0, 0))
                if img.size[0] > texture_size[0] or img.size[1] > texture_size[1]:
                    img = img.resize(texture_size)
                offset = ((texture_size[0] - img.size[0]) // 2, (texture_size[1] - img.size[1]) // 2)
                centered_img.paste(img, offset)

                average_color = calculate_average_color(centered_img)
                img_with_padding = ImageOps.expand(centered_img, border=padding, fill=average_color)

                col = i % atlas_cols
                row = i // atlas_cols
                position = (col * padded_size[0], row * padded_size[1])
                atlas.paste(img_with_padding, position)

                writer.writerow([strip_extension(image_file), position[0] + padding, position[1] + padding])

    atlas.save(output_filename)
    print(f"Texture atlas saved as {output_filename}")
    print(f"Texture position data saved as {csv_filename}")

directory_path = "img"
stitch_textures(directory_path)
