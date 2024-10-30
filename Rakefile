require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"

task default: :test
Rake::TestTask.new do |t|
  t.libs << "test"
  t.pattern = "test/**/*_test.rb"
end

Rake::ExtensionTask.new("faiss") do |ext|
  ext.name = "ext"
  ext.lib_dir = "lib/faiss"
  ruby_include = RbConfig::CONFIG['rubyhdrdir']
  ext.config_options = ["--with-cppflags=-I#{ruby_include}"]
end

task :remove_ext do
  path = "lib/faiss/ext.bundle"
  File.unlink(path) if File.exist?(path)
end

task :remove_obj do
  Dir["ext/faiss/*.o"].each do |path|
    File.unlink(path)
  end
end

Rake::Task["build"].enhance [:remove_ext, :remove_obj]
Rake::Task["compile"].enhance [:remove_obj]

CLEAN.include("vendor/faiss/**/*.o")
