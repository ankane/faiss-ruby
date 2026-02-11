require "bundler/setup"
Bundler.require(:default)
require "minitest/autorun"

class Minitest::Test
  def valgrind?
    ENV["RUBY_MEMCHECK_RUNNING"]
  end
end
