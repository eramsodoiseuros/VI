from PIL import Image
import sys

def convert_ppm_to_jpg(ppm_file_path, jpg_file_path):
    # Open the PPM image
    with Image.open(ppm_file_path) as image:
        # Convert the image to RGB mode (in case it's in a different mode)
        rgb_image = image.convert("RGB")

        # Save the image as JPG
        rgb_image.save(jpg_file_path, "JPEG")


#ppm_file_path = "C:\\Users\\sleim\\Desktop\\MyImage.ppm"
#jpg_file_path = "C:\\Users\\sleim\\Desktop\\output_image.jpg"

def main(ppm_file_path, jpg_file_path):
    convert_ppm_to_jpg(ppm_file_path, jpg_file_path)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python programa.py arquivo.ppm arquivo.jpg")
    else:
        ppm_path = sys.argv[1]
        jpg_path = sys.argv[2]
        main(ppm_path, jpg_path)