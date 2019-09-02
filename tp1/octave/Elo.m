#!/usr/bin/env -S octave -qf
args = argv();
filename = args{1};
matches = dlmread(filename, ' ', 1, 1);
teams = dlmread(filename, ' ', [0 0 0 0]);

K = 60;
Ga = 0.0;
Gb = 0.0;
ratings = 100*ones(teams, 1);

for i = 1 : size(matches)
  teamA = matches(i, 1);
  teamB = matches(i, 3);
  diffa = matches(i, 2) - matches(i,4);
  diffb = matches(i,4) - matches(i, 2) ;
  
  // Obtain G
  if diffa == 1 || diffa == 0 
	  Ga=1.0;
  elseif diffa==2
	  Ga = 1.5;
  else 
	  Ga = (9.0 + diffa)/8.0;
  endif

  if diffb == 1 || diffb == 0 
	  Gb=1.0;
  elseif diffb==2
	  Gb = 1.5;
  else 
	  Gb = (9.0 + diffb)/8.0;
  endif

  // Obtain W
  Wa = 0.5;
  if matches(i, 2) > matches(i,4)
	  Wa = 1.0;
  elseif matches(i, 2) < matches(i,4)
	  Wa = 0.0;
  endif
  Wb = 0.5;
  if matches(i, 2) < matches(i,4)
	  Wb = 1.0;
  elseif matches(i, 2) > matches(i,4)
	  Wb = 0.0;
  endif

  // rating difference
  dra =  (ratings(teamA) - ratings(teamB) +100)*-1;
  drb =  (ratings(teamB) - ratings(teamA))*-1;
  // Win expectancy
  WEa = 1/(10^(dra/400) + 1);
  WEb = 1/(10^(drb/400) + 1);

  // Final rating
  ratings(teamA) = ratings(teamA) + K*Ga*(Wa - WEa);
  ratings(teamB) = ratings(teamB) + K*Gb*(Wb - WEb);
endfor

disp(ratings);


