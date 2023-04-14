
def configure(cfg):
	pass

def construct(ctx):

	ctx.config("type","lib")

	ctx.folder("public include: ../enumgen/incl")

	ctx.fscan("src: ../enumgen/incl")
	ctx.fscan("src: ../enumgen/src")


