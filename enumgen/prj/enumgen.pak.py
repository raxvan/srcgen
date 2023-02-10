
def configure(cfg):
	pass

def construct(ctx):

	ctx.config("type","lib")

	ctx.folder("public include: ../incl")

	ctx.fscan("src: ../incl")
	ctx.fscan("src: ../src")


