function whiten = createwhiten(s)

  sample_rate = 8000;
  
  whiten = zeros(s*sample_rate,1);
  
  t = s*sample_rate
  

  for i = 1:t
    whiten(i) = rand();
  
  end

  whiten = whiten*2-1;

end


%white = rand(8000*10,1)*2-1;
%wavwrite(white,8000,16,'white_noise_sound.wav');
