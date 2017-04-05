tic(); white = rand(8000*1000,1)*2-1; measure = toc();
disp(measure)
disp(size(white))

tic(); whiten = createwhiten(1000); runtime = toc();


disp(runtime)
disp(size(whiten))

ratio = runtime/measure