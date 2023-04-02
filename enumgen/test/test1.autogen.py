
import random

def generate_name_list(lsize):
	chars = "qazwsxedcrfvtgbyhnujmiklop_QAZWSXEDCRFVTGBYHNUJMIKLOP"
	values = set()
	while(len(values) < lsize):
		length = random.randint(1, 48)
		e = ''.join(random.choice(chars) for _ in range(length))
		values.add(e)

	return values

def construct(ctx):

	data = list(generate_name_list(512))

	for a,b in zip(data[0::2], data[1::2]):
		if(random.randint(0, 100) < 40):
			ctx.add(a, alias = b)
		else:
			ctx.add(a)


def generate_cpp_enum(ctx):

	ctx.enumPrefix = "enum_"
	ctx.aliasPrefix = "alias_"

	ctx.namespace = "test_enum"
	ctx.classname = "TestEnum"

	ctx.output = "out"


