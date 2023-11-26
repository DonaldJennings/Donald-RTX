import json
import os
import imageio
import subprocess
import math

def load_scene_from_file(filename):
    with open(filename, 'r') as file:
        scene = json.load(file)
    return scene

def run_raytracer(input_scene, output_ppm):
    # Replace the command with the actual command to run your raytracer
    print("Running raytracer on scene: " + input_scene)
    command = ["../Code/raytracer", input_scene]
    
    with open(output_ppm, 'w') as file:
        process = subprocess.run(command, stdout=file)
    
def update_sphere_center_on_bounce(current_center, radius, velocity, dt, gravity=-2.718):
    new_velocity = velocity + gravity * dt
    new_center = current_center[1] + new_velocity * dt + 0.5 * gravity * dt**2
    
    if new_center - radius < 0:
        new_center = radius
        new_velocity *= -0.95
        
    return [current_center[0], new_center, current_center[2]], new_velocity

    
def generate_scenes(scene, output_folder='./video_scenes'):
    # Clear the output folder
    for f in os.listdir(output_folder):
        os.remove(os.path.join(output_folder, f))
    
    NUM_FRAMES = 288
    initial_velocity_central = -2.7
    initial_velocity_red_marble = -3.0
    initial_velocity_blue_scuffed = -3.2
    intial_velocity_blue_flat = -2.9
    
    current_aperture = scene['camera']['aperture']
    
    # Move Camera around circle at center [0, 0.7, 2.25] by 1 degree each frame
    for i in range(NUM_FRAMES):
        angle = i
        scene['camera']['position'][0] = 0 + 5.25 * math.sin(math.radians(angle))
        scene['camera']['position'][2] = 2.25 + 5.25 * math.cos(math.radians(angle))
        
        central_sphere = scene['scene']['shapes'][0]
        red_marble_sphere = scene['scene']['shapes'][1]
        blue_scuffed_sphere = scene['scene']['shapes'][2]
        blue_flat_sphere = scene['scene']['shapes'][3]
        
        dt = 0.1
        
        scene['scene']['shapes'][0]['center'], initial_velocity_central = update_sphere_center_on_bounce(central_sphere['center'], central_sphere['radius'], initial_velocity_central, dt)
        scene['scene']['shapes'][1]['center'], initial_velocity_red_marble = update_sphere_center_on_bounce(red_marble_sphere['center'], red_marble_sphere['radius'], initial_velocity_red_marble, dt)
        scene['scene']['shapes'][2]['center'], initial_velocity_blue_scuffed = update_sphere_center_on_bounce(blue_scuffed_sphere['center'], blue_scuffed_sphere['radius'], initial_velocity_blue_scuffed, dt)
        scene['scene']['shapes'][3]['center'], intial_velocity_blue_flat = update_sphere_center_on_bounce(blue_flat_sphere['center'], blue_flat_sphere['radius'], intial_velocity_blue_flat, dt)
        
        scene['camera']['lookAt'][0] = scene['scene']['shapes'][0]['center'][0]
        scene['camera']['lookAt'][1] = scene['scene']['shapes'][0]['center'][1]
        scene['camera']['lookAt'][2] = scene['scene']['shapes'][0]['center'][2]
        scene['camera']['aperture'] = current_aperture
        current_aperture += 0.0001
        
        with open(output_folder + 'video_test_frame_{:04d}.json'.format(i), 'w') as file:
            json.dump(scene, file)
            
def convert_ppm_to_mp4(input_folder, output_mp4):
    ppm_files = [f for f in os.listdir(input_folder) if f.endswith(".ppm")]

    images = []
    for ppm_file in ppm_files:
        ppm_path = os.path.join(input_folder, ppm_file)
        image = imageio.imread(ppm_path)
        images.append(image)

    imageio.mimsave(output_mp4, images, fps=24)

def process_scene_folder(scene_folder, ppm_folder):
    for scene_file in os.listdir(scene_folder):
        print("Processing scene: " + scene_file)
        if scene_file.endswith(".json"):
            scene_path = os.path.join(scene_folder, scene_file)
            # Assuming the output PPM filename is based on the input scene filename
            output_ppm = os.path.splitext(scene_file)[0] + ".ppm"
            run_raytracer(scene_path, ppm_folder + output_ppm)
    
if __name__ == '__main__':
    # Generate the scenes
    TEMPLATE_SCENE = './video_test.json'
    scene_folder = '../Video/Scenes/'
    ppm_folder = '../Video/Frames/'
    output_mp4 = 'video.mp4'
    
    with open(TEMPLATE_SCENE, 'r') as file:
        scene = json.load(file)
        print(scene["nbounces"])
        generate_scenes(scene, scene_folder)
    
    process_scene_folder(scene_folder, ppm_folder)
    convert_ppm_to_mp4(ppm_folder, output_mp4)
    # Loop over each scene adn