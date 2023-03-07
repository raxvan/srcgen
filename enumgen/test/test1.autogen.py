
import random



def construct(ctx):

	chars = "qazwsxedcrfvtgbyhnujmiklop_QAZWSXEDCRFVTGBYHNUJMIKLOP"
	values = set()
	for i in range(1024):
		length = random.randint(1, 48)
		e = ''.join(random.choice(chars) for _ in range(length))
		values.add(e)

	for v in values:
		ctx.add(v)


def generate_cpp_enum(ctx):

	ctx.enumPrefix = "enum_"
	ctx.aliasPrefix = "alias_"

	ctx.namespace = "test_enum"
	ctx.classname = "TestEnum"

	ctx.output = "out"
