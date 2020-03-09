require_relative "lib/faiss/version"

Gem::Specification.new do |spec|
  spec.name          = "faiss"
  spec.version       = Faiss::VERSION
  spec.summary       = "Efficient similarity search and clustering for Ruby"
  spec.homepage      = "https://github.com/ankane/faiss"
  spec.license       = "MIT"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@chartkick.com"

  spec.files         = Dir["*.{md,txt}", "{lib,ext,vendor}/**/*"]
  spec.require_path  = "lib"

  spec.required_ruby_version = ">= 2.4"

  spec.add_dependency "rice", ">= 2.2"
  spec.add_dependency "numo-narray"

  spec.add_development_dependency "bundler"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "minitest", ">= 5"
end
