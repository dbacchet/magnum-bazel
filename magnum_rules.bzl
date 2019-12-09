def corrade_gen_resources(name, config, data, **kwargs):
  """
  Generate a cpp file that contains the given resources, to be embedded in the final application
  """
  native.genrule(
    name = name,
    srcs = [config] + data,
    outs = [name + '_resources.cpp'],
    cmd = "./$(execpath //:corrade-rc) {} $(location {}) $@".format(name, config),
    tools = ['//:corrade-rc'],
    # output_to_bindir = True,
    **kwargs
  )
