
import argparse
import sys
import os
import source_generator

def main(args):
	ipath = os.path.abspath(args.path)

	if(os.path.exists(ipath) == False):
		raise Exception(f"Could not find {ipath}")

	baseout, name  = os.path.split(ipath)

	source_generator.generate(name.replace(".autogen.py",""), ipath, baseout)

if __name__ == '__main__':
	user_arguments = sys.argv[1:]
	parser = argparse.ArgumentParser()
	parser.add_argument('path', help='Path to enum description')

	args = parser.parse_args(user_arguments)
	main(args)