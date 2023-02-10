

def configure(cfg):
	cfg.link_if_enabled("../../../ttf/testing.pak.py")

	cfg.link_if_present("../../../dev-platform/prj/dev-platform.pak.py")

def construct(ctx):
	ctx.config("type","lib")

	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl/reflection")
	ctx.fscan("src: ../src/reflection")

	if ctx.module_enabled("testing"):
		ctx.assign("public define:REFLECTION_TESTING")

	if ctx.module_enabled("dev-platform"):
		ctx.assign("public define:REFLECTION_WITH_DEV_PLATFORM")
