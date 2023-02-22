

def construct(ctx):

	ctx.add("EnumValueB")
	ctx.add("EnumValueA")
	ctx.add("EnumValueAB")


def generate_cpp_enum(ctx):

	ctx.enumPrefix = "k"
	ctx.aliasPrefix = "u"

	ctx.namespace = "test_enum"
	ctx.classname = "TestEnum"

	ctx.output = "out"
