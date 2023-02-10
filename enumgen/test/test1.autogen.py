

def make_enum(ctx):

	ctx.add("EnumValueB")
	ctx.add("EnumValueA")
	ctx.add("EnumValueAB")


	ctx.cppPrefix = "k"
	ctx.strPtrfix = "u"

	ctx.namespace = "test_enum"
	ctx.classname = "TestEnum"

	ctx.output = "out"