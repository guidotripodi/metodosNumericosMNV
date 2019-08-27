#!/usr/bin/env -S octave -qf
args = argv();
filename = args{1};
matches = dlmread(filename, ' ', 1, 1);
teams = dlmread(filename, ' ', [0 0 0 0]);

K = 60;
G = 0;
ratings = 100*ones(teams, 1);
newRatings = 100*ones(teams, 1);

for i = 1 : size(matches)
  teamA = matches(i, 1);
  teamB = matches(i, 3);
  diff = abs(matches(i, 2) - matches(i,4));
  if diff == 1 || diff == 0 
	  G=1;
  elseif diff==2
	  G = 1.5;
  else 
	  G = (11 +diff)/8;
  endif

  Wa = 0.5;
  if matches(i, 2) > matches(i,4)
	  Wa = 1;
  elseif matches(i, 2) < matches(i,4)
	  Wa = 0;
  endif
  Wb = 0.5;
  if matches(i, 2) < matches(i,4)
	  Wb = 1;
  elseif matches(i, 2) > matches(i,4)
	  Wb = 0;
  endif
  dr =  ratings(teamA) - ratings(teamB);
  WE = 1/(10^-(dr/400) + 1);
  newRatings(teamA) = ratings(teamA) + K*G*(Wa - WE);
  newRatings(teamB) = ratings(teamB) + K*G*(Wb - WE);
endfor

disp(newRatings);


