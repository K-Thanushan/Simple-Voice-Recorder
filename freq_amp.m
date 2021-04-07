function amplified = freq_amp(fft_sample,start,ending,amp)
    %This function will amplify frequency components between start and
    %ending region
    fft_sample(start:ending) = fft_sample(start:ending)*amp ;
    amplified = fft_sample;
end