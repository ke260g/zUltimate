import imageio, os
import re
# dirname: input directory, 0001.png, 0002.png
# filename: output gif name
def png2gif(dirname, filename):
    images = []
    filenames=sorted((fn for fn in os.listdir(dirname) if fn.endswith('.png')))
    for filename in filenames:
        images.append(imageio.imread(filename))
    imageio.mimsave('filename', images)

root='model'
prev=root
for model in os.listdir(root):
    for anim in os.listdir(os.path.join(root, model)):
        for frames in os.listdir(os.path.join(root, model, anim)):
            dirname = os.path.join(root, model, anim, frames)
            gifname = os.path.join(root, model, anim, frames, frames + '.gif')
            print(frames)
            print(dirname)
            print(gifname)
            png2gif(dirname, gifname)
