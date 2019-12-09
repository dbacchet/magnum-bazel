load("//:magnum_rules.bzl", "corrade_gen_resources")

corrade_gen_resources(
    name = 'cgen_picking',
    config  = 'resources.conf',
    data = ['PhongId.frag',
            'PhongId.vert'],
)

cc_binary(
    name = 'sample_picking',
    srcs = ['PickingExample.cpp',
            ":cgen_picking",
           ],
    deps = ['//:magnum']
)
