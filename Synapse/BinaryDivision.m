%%

printExamples(10);
realStats(13,8)

%************************************************
% Print Examples is a method used for demonstration
% Will print out a random example for binary divsion
% Simply requires 1 paramter to determine how many
% random examples is needed
% Will print out to the console
%***********************************************
function printExamples(repeat)
    while(repeat > 0)
        num = randi(power(2,13) - 1);
        div = randi(power(2,8) - 1);
        itt = randi(8);
        performDivision(num, div, itt);
        repeat = repeat - 1;
    end
    
end

%*********************************************************
% realStats finds the average percent error and average
% standard deviation for all possible itterations for the 
% function binaryDivsion
% Will print out chart
% Requires the maximum power of the numerator (2^n - 1)
% and the maximum power of the denominator (2^n - 1)
%********************************************************
function realStats(numeratorPower, divisorPower)
    repeating = 1000; %takes 1000 random combos
    meanArrays = zeros(1, divisorPower);
    stdArrays = zeros(1, divisorPower);
    percentErrorArrays = zeros(1, repeating);
    
    for x = 1:divisorPower %looping through all possible itterations
       for y = 1:repeating %looping through all 1000 random combos
            num = randi(power(2, numeratorPower) - 1); %random numberator
            div = randi(power(2, divisorPower) - 1); %random denominator
            [quotient] = binaryDivision(num, div, x);
            expected = num / div;
            percentErrorArrays(y) = abs(expected - quotient)/expected * 100;
       end
       meanArrays(x) = mean(percentErrorArrays);
       stdArrays(x) = std(percentErrorArrays);
    end
    
    figure;
    p1 = plot(1:divisorPower, meanArrays);
    hold on
    p2 = plot(1:divisorPower, stdArrays);
    xlabel('Iterations')
    ylabel('Percent Error')
    title('Statistics for Binary Division');
    xlim([1, divisorPower])
    legend('Mean Percent Error', 'Standard Deviation')
    p1.Marker = 'x';
    p2.Marker = '+';
    hold off
end

%***********************************************************
% This function is for demonstration purposes.
% It will print out the logic that the function binaryDivsion
% Goes through and how it estimates its division
% Must be able to support the parameters of the binaryDivision
% Will print output to the console
%************************************************************
function performDivision(numerator, divisor, itter)
    [estimatedAnswer, upperBoundArray, lowerBoundArray, averageArray, decisionTree, MSB] = binaryDivision(numerator, divisor, itter);
    actualAnswer = numerator / divisor;
    binaryDivisor = dec2bin(divisor);
    binaryNumerator = dec2bin(numerator);
    format long
    fprintf("\n\t\t%.2f Divided by %.2f is %.2f\n", numerator, divisor, actualAnswer);
    fprintf("\t\tThe Estimated Value of this Operation is %.2f\n", estimatedAnswer);
    fprintf("\t\tThe Numerator in Binary is %s and the Divisor is %s\n", binaryNumerator, binaryDivisor);
    fprintf("\t\tThe Algorithm is Set to Iterate %.2f Times\n", itter);
    fprintf("\t\tHeres How its Done\n");
    fprintf("\n\t\tThe Most Significant Bit in the Divisor is 2^%d", MSB);
    fprintf("\n\t\tThe First Lower Bound is Found by Shifting the Numerator to the Right by %d", MSB);
    fprintf("\n\t\tThe First Upper Bound is Found by Shifting the Numerator to the Right by %d", (MSB - 1));
    fprintf("\n\t\tThen the Average is Taken Between the 2 to Find the First Guess");
    fprintf("\n\n\t\tIteration %d:\t%.2f ----------> %.2f <---------- %.2f\n", 1, lowerBoundArray(1), averageArray(1), upperBoundArray(1));
    fprintf("\n\t\tFrom Here, the Algorithm will Look at the Next Bit in the Divisor to Determine Which Side to Look at\n\t\tUntil it has Reached the End of Iterations, or there are No More Bits to Look at");
    fprintf("\n\t\tIf the Bit is On, it will Look in the Lower Half");
    fprintf("\n\t\tIf the Bit is Off, it will Look in the Upper Half\n");
    z = 2;
    while(z <= length(decisionTree))
        switch decisionTree(z - 1)
            case 0
                fprintf("\n\t\tThe Next Bit is Off, so the Average Becomes the Upper Bound of the Search");
            case 1
                fprintf("\n\t\tThe Next Bit is On, so the Average Becomes the Lower Bound of the Search");
        end
        fprintf("\n\t\tIteration %d:\t%.2f ----------> %.2f <---------- %.2f\n", z, lowerBoundArray(z), averageArray(z), upperBoundArray(z));
        z = z+1; 
    end
    fprintf("\n\t\tThe Percent Error is: %.2f%%", abs(estimatedAnswer - actualAnswer) / actualAnswer * 100);
    fprintf("\n\t\t---------------------------------------------------------------------------\n");
    
    
end
%*********************************************
% This function finds the statistical data for the 
% function binaryDivision.  It runs through ALL possible combinations
% This one is slow.  I'm leaving it here just to not delete previous work
% This method is not used, but its here
% Requires 2 inputs: numeratorPower and divisorPower
% Stats function will find all possible quotients where the max numerator
% can be 2^numeratorPower and the divisor can be 2^divisorPower
% Inputs must be greater than 0
% Example: stats(13,8)
%*********************************************
function stats(numeratorPower, divisorPower)
    fprintf('\n****************Starting New Statistics Calculation****************\n')
    maxNumerator = power(2, numeratorPower);
    maxDivisor = power(2, divisorPower);
    PercentErrorArray = zeros(1, maxNumerator*maxDivisor);
    
    %runs through all possible iterations, numerators and divisors
    for k = 1:divisorPower
        for i = 1:maxNumerator 
            for j = 1:maxDivisor
                [quotient] = binaryDivision(i,j,k);
                expected = i/j;
                percentError = abs((expected - quotient))/expected * 100;
                PercentErrorArray((i-1)*maxDivisor+j) = percentError;
            end
        end
        
        % For each iteration, will make historgram of the percent error
        histogram(PercentErrorArray,100)
        ylabel('Number of Occurences of Each Percent Error')
        xlabel('PercentError')
        title('Allowed Iterations = ', k)
        figure;
        
        % Finding and printing to console standard deviation and mean
        stdPercent = std(PercentErrorArray);
        meanPercent = mean(PercentErrorArray);
        fprintf('Iteration: %d\n\tStandard Devation of Percent Error: %d\n\tMean of Perecent Error: %d \n\n', k,stdPercent, meanPercent)

    end
end


%********************************
% This is a function that runs Binary Division Using Binary Search Tree Logic
% Requires three inputs: numerator, divisor, itter (number iterations) 
% All vars must be greater than 1
% Divisor must be smaller than 2^64-1
% Will output the estimated result of numerator/ divisor
% Will also output Arrays of all of the upperbounds, lowerbounds
% averages, and the decisions it took along the way.  These are for
% demonstration purposes
% Example use: [result, upperArray, lowerArray, averageArray, decisionTree, MSB] = binaryDivision(5000, 50, 4) 
% (5000 divided by 50, will iterate 4 times)
%*********************************
function [quotient, upperBoundArray, lowerBoundArray, averageArray, decisionTree, MSB] = binaryDivision(numerator, divisor, itter)
    MSB = 64; %max number of bits divisor can be
    boolMSB = 0; %boolean logic variable
    upperBoundArray = zeros(1, itter);
    lowerBoundArray = zeros(1, itter);
    averageArray = zeros(1, itter);
    decisionTree = zeros(1, itter - 1);
    while boolMSB == 0 %finds first most significant bit
        if power(2, MSB) <= divisor
            boolMSB = 1;
        else
            MSB = MSB - 1;
        end
    end
    
    counter = 1; %var needed for comparison calcs
    upperBound = numerator / power(2,MSB); %lowerBound of Search, can be done with a shift
    upperBoundArray(counter) = upperBound; 
    lowerBound = numerator/ power(2,MSB+1); %upperBound of Search, can be done with a shift
    lowerBoundArray(counter) = lowerBound;
    average = (upperBound + lowerBound)/2; %average of search)
    averageArray(counter) = average;
    itter = itter-1; %lowers iteration count by one since one search has finished
    divisor2 = divisor - power(2,MSB); %subrating out bit already looked at, can be done with a shift
    
    
    %will iterate like a binary search tree to find approximate answer
    while(itter > 0) %will only iterate as many levels as the user inputs
        if(divisor2 == 0 && power(2, MSB-counter)>0) %if there are no more bits left on, will take upperhalf
            lowerBound = average;
            decisionTree(itter) = 0;
        elseif(2*divisor2 <= power(2,MSB-counter)) %if bit is off, then go to upper half
            lowerBound = average;
            decisionTree(itter) = 0;
        else
            divisor2 = divisor2 - power(2,MSB-counter); %taking out bit that was already looked at
            upperBound =  average; %if bit is on, then go lower half
            decisionTree(itter) = 1;
        end
        
        average = (upperBound+lowerBound)/2; %finding average of new range, can be done with a shift
        upperBoundArray(counter) = upperBound;
        lowerBoundArray(counter) = lowerBound;
        averageArray(counter) = average;
        itter = itter - 1; %lowering iteration count by one, this levels search has finished
        counter = counter + 1;
    
    end
    quotient = average; %outputs estimate
end
