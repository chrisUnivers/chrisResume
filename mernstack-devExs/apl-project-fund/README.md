## Notes
  * The purpose of this application is to open and track the funding progress of a project.
  * For now it's a simple modification to the tutorial linked below. I plan on adding authentication and different user groups. The tutorial uses an older version of tailwindcss. 
    + Tutorial video
      - https://www.youtube.com/watch?v=4nKWREmCvsE
    + New tailwind updated
      - https://tailwindcss.com/docs/installation/using-postcss

## How to run
  * node --env-file=config.env server
    + in one terminal in the server folde
  * npm run dev
    + in a different terminal in the cliend folde

## Issues if following old guides
  * npx problems. See comment on npx cli
    + https://stackoverflow.com/questions/79380514/problem-installing-tailwindcss-with-vite-after-npx-tailwindcss-init-p-comman/79380522#79380522
    + Solution
      - https://tailwindcss.com/docs/installation/using-vite
    + See comments
      - https://stackoverflow.com/questions/73474335/tailwind-config-js-file-not-being-generated
      - https://stackoverflow.com/questions/78908521/npx-tailwind-css-init-not-working-in-powershell
    + solution use without `init -p`