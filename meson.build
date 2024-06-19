project(
  'gover', 'c',
  version: '1.0',
  license: 'MIT',
  meson_version: '>= 1.0.0',
)

gover_package = 'gover-1.0'

gtk_layer_shell_dep = dependency('gtk4-layer-shell',
        required: true,
        fallback : ['gtk4-layer-shell', 'gtk_layer_shell'])

gtk = dependency('gtk4')


deps = [
  gtk_layer_shell_dep,
  gtk,
  ]

LIBS = [gtk]

subdir('src')
