clc; clear all; close all;

disp('Bisection FP...');
tic;

    Out = BFPSolver(-5,5);

disp(['Root = ' num2str(Out)])
a=toc;
disp(['Elapsed time ' num2str(a)])


%% 

disp('Bisection FP MEX...');
tic;

    Out = BFPSolver_mex(-5,5);

disp(['Root = ' num2str(Out)])
b=toc;
disp(['Elapsed time ' num2str(b)])
disp(['MEX is ' num2str(a/b) ' times faster'])


%% 
% 
% 
% 
% tic;
% Out = fsolve(@(x) (x-1.5).^3-1,-1);
% disp(Out);
% b=toc;
% disp(['mex is ' num2str(b/a) ' times faster than fsolve'])
% 
