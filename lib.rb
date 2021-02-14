def lines file, line1, line2
  File.read(file).match(/^#{line1}$(.*?)^#{line2}$/m)[1].strip
    .split(/\n/)
    .filter {|s| !(s =~ /\s*\/\/\s*f?printf\(/) }
    .join("\n")
end
