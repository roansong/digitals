function r = mycorr(x,y)
	% readability >> succinctness

	sx  = sum(x);
	sy  = sum(y);
	sxy = sum(x.*y);    % elementwise multiplication
	sx2 = sum(x.^2);    % elementwise squares
	sy2 = sum(y.^2);
	n   = max(size(x)); % just in case we get some column/row vector mixups
	num = sxy - sx*sy/n;
	den = sqrt((sx2-(sx.^2)/n)*(sy2-(sy.^2)/n));

	r = -2; % default, in case correlation could not be performed

	if(den != 0)
		r = num/den;
	end
end