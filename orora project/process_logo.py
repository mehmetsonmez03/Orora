
import os
from PIL import Image, ImageDraw

def process_logo(input_path, output_path):
    try:
        # Load the image
        img = Image.open(input_path).convert("RGBA")
        width, height = img.size
        
        # Create a high-quality circular mask
        # We use a slightly smaller circle to ensure no edge artifacts
        mask = Image.new('L', (width, height), 0)
        draw = ImageDraw.Draw(mask)
        
        # Draw a white circle on the black mask
        # We leave a tiny 2px margin to ensure smoothness
        draw.ellipse((5, 5, width-5, height-5), fill=255)
        
        # Apply the mask to the image
        output = Image.new('RGBA', (width, height), (0, 0, 0, 0))
        output.paste(img, (0, 0), mask=mask)
        
        # Save as PNG
        output.save(output_path, "PNG")
        print(f"SUCCESS: Logo processed and saved to {output_path}")
        return True
    except Exception as e:
        print(f"ERROR: {str(e)}")
        return False

if __name__ == "__main__":
    # Source is the uploaded artifact JPG
    src = r"C:\Users\mehmet\.gemini\antigravity\brain\c72ae4b4-c185-48a2-b95f-c453211d8284\uploaded_media_0_1769730164224.jpg"
    dest = r"C:\orora project\logo\orora.png"
    
    # Ensure directory exists
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    
    process_logo(src, dest)
