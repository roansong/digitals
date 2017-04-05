x10 = rand(1,10); y = rand(1,10);

i = 8

timings = zeros(size(i)(2),4);

for k = 1:i
	a = rand(1,10^k);
	b = rand(1,10^k);

	tic(); corr(a,b);   t1 = toc();
	tic(); mycorr(a,b); t2 = toc();
	sp = t1/t2;

	timings(k,1) = t1;
	timings(k,2) = t2;
	timings(k,3) = sp;
	timings(k,4) = 10^k;
end

