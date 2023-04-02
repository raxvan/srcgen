
import random

def construct(ctx):

	ctx.add("member1", type_alias = "int", type_default = "1")
	ctx.add("member2", type_alias = "float", type_default = "2.0f")


def generate_cpp_enum(ctx):

	ctx.enumPrefix = "enum_"
	ctx.aliasPrefix = "alias_"

	ctx.namespace = "test_enum"
	ctx.classname = "TestEnum2"

	ctx.output = "out"


def generate_cpp_struct(ctx):

	ctx.namespace = "test_struct"
	ctx.classname = "TestStruct2"
	ctx.output = "out"

	ctx.type_alias = "type_alias"

