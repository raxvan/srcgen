
def configure(cfg):
	pass

def construct(ctx):

	ctx.config("type","lib")

	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl")


def configure(cfg):
	cfg.link("../../../ttf/testing.pak.py").enable()
	cfg.link("enumgen.pak.py")

def construct(ctx):
	ctx.config("type","exe")

	ctx.fscan("src: ../test/")
