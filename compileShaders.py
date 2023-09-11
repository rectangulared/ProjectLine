import os
import subprocess

vert_shaders_path = '.\\shaders\\vertex\\'
file_list = os.listdir(vert_shaders_path)
print(file_list)

for file in file_list:
    if '.spv' not in file:
        subprocess.run(['glslc', vert_shaders_path + file, '-o', vert_shaders_path + file + '.spv'])
    
frag_shaders_path = '.\\shaders\\fragment\\'
file_list = os.listdir(frag_shaders_path)
print(file_list)

for file in file_list:
    if '.spv' not in file:
        subprocess.run(['glslc', frag_shaders_path + file, '-o', frag_shaders_path + file + '.spv'])

