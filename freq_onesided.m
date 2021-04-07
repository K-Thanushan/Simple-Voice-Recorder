function oneside = freq_onesided(fft_sample,len)
    %This function will return absolute and one sided frequency components
    part2 = abs(fft_sample/len) ;
    part1 = part2(1:len/2+1);
    part1(2:end-1) = 2*part1(2:end-1);
    oneside = part1;
end