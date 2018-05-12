function value = matrix_precision(A)
% MATRIX_PRECISION Calculate the mean manhatten distance of a matrix
%
% To use open command line and run "Make" in directory to build program
% "mean_manhatten"
%
% The code for this and the subprogram to create it could be converted
% to produce a histogram of distances if needed 
% Written by Ben Perlin 2018

    filename = tempname;                    % generate a unique temporary filename
    csvwrite(filename, logical(A));         % write data to that file
    [status, cmdout] = system(['./mean_manhatten ', filename]); % execute command
    
    delete filename;
    
    if (status ~= 0) 
        error(['program "mean_manhatten" failed: ',  cmdout]);
    end
    
    % todo parse
    value = str2double(cmdout);
end