require_relative "lib/faiss/version"

Gem::Specification.new do |spec|
  spec.name          = "faiss"
  spec.version       = Faiss::VERSION
  spec.summary       = "Efficient similarity search and clustering for Ruby"
  spec.homepage      = "https://github.com/ankane/faiss"
  spec.license       = "MIT"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@ankane.org"

  spec.files         = Dir["*.{md,txt}", "{lib,ext}/**/*", "vendor/faiss/LICENSE", "vendor/faiss/faiss/**/*.{cpp,h}"]
  spec.require_path  = "lib"
  spec.extensions    = ["ext/faiss/extconf.rb"]

  spec.required_ruby_version = ">= 2.4"

  spec.add_dependency "rice", ">= 2.2"
  spec.add_dependency "numo-narray"
end
