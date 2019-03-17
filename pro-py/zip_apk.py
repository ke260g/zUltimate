import os
import zipfile
import argparse

# apk: output apk path
# dir: input  dir path, unziped from an apk
def zipApk(apk, dir):
  zipf = zipfile.ZipFile(apk, 'w', compression=zipfile.ZIP_DEFLATED)
  os.chdir(dir)
  
  for root, dirs, files in os.walk('.'):
    for file in files:
      if root == '.' and file == 'resources.arsc':
      	zipf.write(os.path.join(root, file), compress_type=zipfile.ZIP_STORED)
      else:
      	zipf.write(os.path.join(root, file))
  
  zipf.close()

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='zip a apk with all files deflated')
  parser.add_argument('--output', help="path to output apk")
  parser.add_argument('--input', help="path to input folder, which is unziped from an apk")
  args = parser.parse_args()
  if args.output == None or args.input == None:
    parser.print_help()
    exit(-1)

  zipApk(args.output, args.input)
  