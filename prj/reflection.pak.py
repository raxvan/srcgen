

def configure(cfg):
	cfg.link_if_enabled("../../ttf/testing.pak.py")

def construct(ctx):
	ctx.config("type","lib")

	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl/reflection")
	ctx.fscan("src: ../src/reflection")

	if ctx.module_enabled("testing"):
		ctx.assign("public define:REFLECTION_TESTING")
