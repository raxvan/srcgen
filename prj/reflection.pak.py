

def configure(cfg):
	cfg.link_if_enabled("../../ttf/testing.pak.py")

	cfg.link_if_present("../../dev-platform/prj/dev-platform.pak.py")

def construct(ctx):
	ctx.config("type","lib")

	ctx.folder("public include: ../cppreflect/incl")

	ctx.fscan("src: ../cppreflect/incl/reflection")
	ctx.fscan("src: ../cppreflect/src/reflection")

	if ctx.module_enabled("testing"):
		ctx.assign("public define:REFLECTION_TESTING")

	if ctx.module_enabled("dev-platform"):
		ctx.assign("public define:REFLECTION_WITH_DEV_PLATFORM")
