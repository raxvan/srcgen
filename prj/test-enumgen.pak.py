
def autogenerate():
	return [
		"../enumgen/test/test1.autogen.py",
		"../enumgen/test/test2.autogen.py",
		"../enumgen/test/other/test1.autogen.py"
	]

def configure(cfg):
	cfg.link("../../ttf/testing.pak.py").enable()
	cfg.link("enumgen.pak.py")

def construct(ctx):
	ctx.config("type","exe")

	ctx.fscan("src: ../enumgen/test/")

