
import random

def construct(ctx):

	ctx.add("mint", type_alias = "ss::IntProperty", type_default = "1")
	ctx.add("mfloat", type_alias = "ss::FloatProperty", type_default = "1.0f")

	ctx.add("mint_array", type_alias = "ss::IntArrayProperty", type_default = "{}")
	ctx.add("mfloat_array", type_alias = "ss::FloatArrayProperty", type_default = "{}")

	ctx.add("mstring", type_alias = "ss::StringProperty", type_default = "{}")


def generate_cpp_enum(ctx):

	ctx.enumPrefix = "e_"
	ctx.aliasPrefix = "p_"

	ctx.namespace = "test"
	ctx.classname = "TestEnum"

	ctx.output = "out"


def generate_cpp_struct(ctx):

	ctx.namespace = "test"
	ctx.classname = "TestStruct"
	ctx.output = "out"

	ctx.type_alias = "type_alias"
	ctx.type_default = "type_default"
