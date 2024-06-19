gnome = import('gnome')
pkg = import('pkgconfig')

libgover_src = [
  'gover.c',
  'gover-canvas.c',
]

libgover_headers = [
  'gover.h',
  'gover-canvas.h',
]

libgover = library(gover_package, libgover_src,
  dependencies : deps, install: true)

pkg.generate(libgover,
  name: meson.project_name(),
  filebase: gover_package,
  subdirs: gover_package,
  description: 'A bar library for writing status bars',
  requires: LIBS,
  )

if get_option('introspection')
  gir_args = [
    '--accept-unprefixed',
    ]

  gover_gir = gnome.generate_gir(
    libgover,
    sources: libgover_src + libgover_headers,
    export_packages: [gover_package],
    header: 'gover/gover.h',
    namespace: 'Gover',
    nsversion: '1.0',
    symbol_prefix: ['g_gover', 'gover'],
    identifier_prefix : ['Gover'],
    includes: [ 'Gtk-4.0'],
    dependencies: deps,
    extra_args: gir_args,
    fatal_warnings: true,
    install: true,
  )
endif

if get_option('vala')
  gnome.generate_vapi(gover_package,
    sources: gover_gir[0],
    packages: [ 'gtk4'],
    install: true)
endif

executable('gover',
  libgover_src + 'main.c',
  dependencies: deps,
  link_args: ['-Wl,--export-dynamic'],
  install: true
)
