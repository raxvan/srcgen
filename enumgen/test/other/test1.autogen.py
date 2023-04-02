
def construct(ctx):
	for i in range(256):
		ctx.add("Test_" + str(i), type_alias = "ctype_wrap", type_default = "{}")
