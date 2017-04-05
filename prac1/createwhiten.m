function whiten = createwhiten(time)
  sample_rate = 8000;  
  whiten = zeros(time*sample_rate,1); 
  samples = time*sample_rate
  for i = 1:samples
    whiten(i) = rand(); 
  end
  whiten = whiten*2-1; %fit into .wav bounds
end


%white = rand(8000*10,1)*2-1;
%wavwrite(white,8000,16,'white_noise_sound.wav');
