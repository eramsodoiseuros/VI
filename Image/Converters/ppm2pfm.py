import cv2
import numpy as np
import sys
def ppm_to_pfm(ppm_file, pfm_file):
    # Read the PPM image using OpenCV
    image = cv2.imread(ppm_file, cv2.IMREAD_UNCHANGED)

    # Extract image dimensions
    height, width, channels = image.shape

    # Convert pixel values to float32 and rearrange channel order
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB).astype(np.float32)

    # Normalize pixel values to the range [0, 1]
    image /= 255.0

    # Create the PFM header
    pfm_header = f'PF\n{width} {height}\n-1.0\n'

    # Write the PFM file
    with open(pfm_file, 'wb') as pfm:
        pfm.write(pfm_header.encode())
        image.tofile(pfm)

#ppm_file_path = "C:\\Users\\sleim\\Desktop\\MyImage.ppm"
#pfm_file_path = "C:\\Users\\sleim\\Desktop\\PFM.pfm"

#ppm_to_pfm(ppm_file_path, pfm_file_path)

def main(ppm_file_path, pfm_file_path):
    ppm_to_pfm(ppm_file_path, pfm_file_path)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python programa.py arquivo.ppm arquivo.exr")
    else:
        ppm_path = sys.argv[1]
        jpg_path = sys.argv[2]
        main(ppm_path, jpg_path)