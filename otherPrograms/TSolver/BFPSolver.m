function Out = BFPSolver(xmin,xmax)  
%#codegen
coder.inline('never');

% Bisection False Position Method Algorithm
% Find the roots of F(x) = 0 given a search interval [xmin xmax];
tole = 1e-6;     % Tolerance level

% Make sure the function has at least one zero
ffm=fun(xmin);
ffM=fun(xmax);
if (ffm > 0 && ffM > 0) || (ffm < 0 && ffM < 0)
    error('The function might not have zeros');
end

% Bisection algortihm
iter=1;
% Loop
if ffm > 0 
    % Decrea function
    xmid = 0.5*(xmin+xmax);
    fmid = fun(xmid);
    while ((ffm-ffM) > tole) && abs(fmid)>tole
        xmid = xmin - ((xmax-xmin)/(ffM - ffm)) *ffm;
        fmid = fun(xmid);
        if fmid > 0
            xmin = xmid;
            ffm=fun(xmin);
        else
            xmax = xmid;
             ffM=fun(xmax);
        end
        iter=iter+1;
    end
else
    % Increas function
    xmid = 0.5*(xmin+xmax);
    fmid = fun(xmid);
    while (ffM-ffm) > tole  && abs(fmid)>tole
        xmid = xmin - ((xmax-xmin)/(ffM - ffm)) *ffm;
        fmid = fun(xmid);
        if fmid > 0
            xmax = xmid;
            ffM=fun(xmax);
        else
            xmin = xmid;
            ffm=fun(xmin);
        end
        iter=iter+1;
    end
end

Out = xmid;
end
