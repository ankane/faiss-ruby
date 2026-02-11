require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"
require "ruby_memcheck"

test_config = lambda do |t|
  t.pattern = "test/**/*_test.rb"
end
Rake::TestTask.new(&test_config)

namespace :test do
  RubyMemcheck::TestTask.new(:valgrind, &test_config)
end

task default: :test

Rake::ExtensionTask.new("faiss") do |ext|
  ext.name = "ext"
  ext.lib_dir = "lib/faiss"
end

task :remove_ext do
  Dir["lib/faiss/ext.{bundle,so}"].each do |path|
    File.unlink(path)
  end
end

task :remove_obj do
  Dir["ext/faiss/*.o"].each do |path|
    File.unlink(path)
  end
end

Rake::Task["build"].enhance [:remove_ext, :remove_obj]
Rake::Task["compile"].enhance [:remove_obj]

CLEAN.include("vendor/faiss/**/*.o")
