
def autogenerate():
	return [
		"../test/test1.autogen.py",
	]

def configure(cfg):
	cfg.link("../../../ttf/testing.pak.py").enable()
	cfg.link("serializer.pak.py")

def construct(ctx):
	ctx.config("type","exe")

	ctx.fscan("src: ../test/")

