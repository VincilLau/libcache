set_project("libcache")
set_version("0.0.0")
set_xmakever("2.7.3")
set_languages("c++17")

add_rules("mode.debug", "mode.release")

target("libcache")
    set_kind("static")
    add_includedirs("include", "src")
    add_files("src/**.cpp")
