
def configure(cfg):
	cfg.link_if_enabled("../../ttf/testing.pak.py")
	cfg.link("../../dev-platform/prj/dev-platform.pak.py")
	cfg.link("enumgen.pak.py")

def construct(ctx):

	ctx.config("type", "lib")

	ctx.folder("public include: ../serializer/incl")

	ctx.fscan("src: ../serializer/incl")
	ctx.fscan("src: ../serializer/src")

	if ctx.module_enabled("testing"):
		ctx.assign("public define:SERIALIZER_TESTING")


