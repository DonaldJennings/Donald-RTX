import json
import os
import imageio
import subprocess

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
    
def generate_scenes(scene, output_folder='./video_scenes'):
    # move the camera up 0.05 units per frame for 100 frames
    for i in range(50):
        scene['camera']['position'][1] += 0.01
        # save the scene to a file named video_test_frame_0000.json
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