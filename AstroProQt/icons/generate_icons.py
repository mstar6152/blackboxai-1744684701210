from PIL import Image, ImageDraw, ImageFont
import os

# Create icons directory if it doesn't exist
if not os.path.exists('icons'):
    os.makedirs('icons')

# Planet symbols and colors
planets = {
    'sun': ('☉', '#FFB800'),     # Golden
    'moon': ('☽', '#C0C0C0'),    # Silver
    'mars': ('♂', '#FF0000'),     # Red
    'mercury': ('☿', '#00FF00'),  # Green
    'jupiter': ('♃', '#FFFF00'),  # Yellow
    'venus': ('♀', '#800080'),    # Purple
    'saturn': ('♄', '#000080'),   # Dark Blue
    'rahu': ('☊', '#808080'),     # Gray
    'ketu': ('☋', '#404040'),     # Dark Gray
}

# Icon settings
size = (32, 32)
background = (30, 30, 30, 0)  # Transparent background

for planet, (symbol, color) in planets.items():
    # Create new image with transparency
    img = Image.new('RGBA', size, background)
    draw = ImageDraw.Draw(img)
    
    # Try to load a font that supports astronomical symbols
    try:
        font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 24)
    except:
        font = ImageFont.load_default()
    
    # Calculate text size and position
    text_bbox = draw.textbbox((0, 0), symbol, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    text_height = text_bbox[3] - text_bbox[1]
    x = (size[0] - text_width) // 2
    y = (size[1] - text_height) // 2
    
    # Draw symbol
    draw.text((x, y), symbol, fill=color, font=font)
    
    # Save icon
    img.save(f'icons/{planet}.png')

# Create additional icons
icons = {
    'chart': '📊',
    'export': '📤',
    'search': '🔍',
    'zoom': '🔎',
}

for name, symbol in icons.items():
    img = Image.new('RGBA', size, background)
    draw = ImageDraw.Draw(img)
    
    try:
        font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 24)
    except:
        font = ImageFont.load_default()
    
    text_bbox = draw.textbbox((0, 0), symbol, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    text_height = text_bbox[3] - text_bbox[1]
    x = (size[0] - text_width) // 2
    y = (size[1] - text_height) // 2
    
    draw.text((x, y), symbol, fill='#FFFFFF', font=font)
    img.save(f'icons/{name}.png')

print("Icons generated successfully!")
