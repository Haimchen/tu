count = 0

(1..100).each do |n|
	if (n % 2 == 0) || (n % 3 == 0) || (n % 5 == 0)
		count += 1
	end
end

puts count