---
toc: false
theme: [air, ocean-floor, wide]
---


### Amanda And Chris
# CSC 411 Final Project
## British Columbia Government Contract Spending

```js
// import {DonutChart} from "./components/donutChart.js";
import {bigNumber} from "./components/bigNumber.js";
import {greatNumber} from "./components/bigNumber.js";
import {geniusNumber} from "./components/bigNumber.js";

import {DonutsToEat} from "./components/donutsToEat.js";

const hotelData = FileAttachment("data/hotelData.csv").csv({typed: true});
const ministryFile = FileAttachment("./data/ministry-contract-em.csv").csv({typed: true});
```

```js
// Radio button input to choose market segment
const defaultSelected = ["Corporate"];
const pickMarketSegmentInput = Inputs.checkbox(
  ["All"].concat(hotelData.filter((d) => d.MarketSegment != "Complementary").map((d) => d.MarketSegment)),
  {
    label: "Booking type:",
    value: defaultSelected,
    unique: true
  }
);
const pickMarketSegment = Generators.input(pickMarketSegmentInput);


const defaultProvinceLt = ["British Columbia"];
const pickProvSegmentInput = Inputs.checkbox(
  ministryFile.filter((d) => d.Province != "NOProvince").map((d) => d.Province),
  {
    value: defaultProvinceLt,
    sort: true,
    unique: true
  }
);
const pickProvSegment = Generators.input(pickProvSegmentInput);
const yrsInput = Inputs.range([2012, 2018], {step: 1, value: 0, width: 150});
yrsInput.querySelector("input[type=number]").remove();
const selectedYr = Generators.input(yrsInput);
```

```js
const countryOrder = ["PRT", "ESP", "GBR", "DEU", "FRA", "IRL", "AUT", "Other", "Unknown"];
const shortIndustry = ["Community Services", "Infrastructure", "Business", "GovernmentLaw", "Environment", "Health", "CultureTourism"];

const countryColors = [
  "#4269d0",
  "#efb118",
  "#ff725c",
  "#6cc5b0",
  "#3ca951",
  "#ff8ab7",
  "#a463f2",
  "#97bbf5",
  "#9c6b4e"
];

const shortIndrColors = [
  "#4269d0",
  "#efb118",
  "#ff725c",
  "#6cc5b0",
  "#3ca951",
  "#ff8ab7",
  "#9c6b4e"
  ];

const showMeColors = ["#ff8ab7", "#6cc5b0", "#a463f2"];
```

${pickProvSegmentInput}

<div class="grid grid-cols-4">
  <div class="card grid-rowspan-2">
    ${resize(width => DonutsToEat(lftTorender, {centerText: lftText[0], width, colorDomain: countryOrder, colorRange: shortIndrColors}))}
  </div>
  <div class="card grid-rowspan-2">
    ${resize(width => DonutsToEat(midTorender, {centerText: midText[0], width, colorDomain: countryOrder, colorRange: shortIndrColors}))}
  </div>
  <div class="card grid-rowspan-2">
    ${resize(width => DonutsToEat(rhtTorender, {centerText: rhtText[0], width, colorDomain: countryOrder, colorRange: shortIndrColors}))}
  </div> 
  <div class="card grid-rowspan-1">
    ${resize((width) => greatNumber(noDefaultPrvName, `Is shown whenever a cell has no provinve selected or the selected province has missing data.`, width))}
  </div>
  
  <div class="card grid-rowspan-1">
    <!-- <div>Select The year: ${yrsInput}</div> -->
    <div style="display: flex; justify-content: center; align-items: center; font-weight: bold;">
      <div>Data from: ${selectedYr}</div>
    </div>
    <div style="display: flex; align-items: center;">
        <!-- <div>From 2012</div> -->
        ${yrsInput}
        <div style="padding-left: 0.5rem;"></div>
    </div>
  </div>

  <div class="card grid-rowspan-1">
    ${resize((width) => geniusNumber(lftText, `CAD$${(lftSltdPrvYrIndrRevSum / 1000000).toFixed(2)}`, width))}
  </div>
  <div class="card grid-rowspan-1">
    ${resize((width) => geniusNumber(midText, `CAD$${(midSltdPrvYrIndrRevSum / 1000000).toFixed(2)}`, width))}
  </div>
  <div class="card grid-rowspan-1">
    ${resize((width) => geniusNumber(rhtText, `CAD$${(rhtSltdPrvYrIndrRevSum / 1000000).toFixed(2)}`, width))}
  </div>
</div>







<!-- [{name: x, value: [[a, b], [c, d]]}, {name: y, value:[[e, f], [g, h], [o, w]]}]
[{name: x, txi: a, tj: b}, {name: x, ti: c, tj: d},
{name: y, ti: e, tj: f}, {name: y, ti: g, tj: h}, {name: y, ti: o, tj: w}] -->


<!-- <div class="grid card" style="height: 250px">
    ${resize((width, height) => arrivalLineChart(width, height))}
</div> -->

<!-- <div class="grid card" style="height: 350px">
    ${resize((width, height) => selectedProvRevSumBySeason(width, height))}
</div> -->


<div class="grid grid-cols-3">
  <div class="card grid-colspan-1">
    <h2>Total Amount of Revenue From Government Contracts</h2>
    <h3>${lftText[0]} from 2012 To 2018</h3>
    ${resize((width) => lftCircleRevAll(width, lftText[0]))}
  </div>
  <div class="card grid-colspan-1">
    <h2>Total Amount of Revenue From Government Contracts</h2>
    <h3>${midText[0]} from 2012 To 2018</h3>
    ${resize((width) => midCircleRevAll(width, midText[0]))}
  </div>
  <div class="card grid-colspan-1">
    <h2>Total Amount of Revenue From Government Contracts</h2>
    <h3>${rhtText[0]} from 2012 To 2018</h3>
    ${resize((width) => rhtCircleRevAll(width, rhtText[0]))}
  </div>
</div>

```js
// Filtered data for selected market segment
//bookingCountryTopN
const bookingsByMarketSegment =
  pickMarketSegment == "Direct"
    ? hotelData.filter((d) => d.MarketSegment != "Complementary")
    : hotelData.filter((d) => d.MarketSegment == pickMarketSegment && d.MarketSegment != "Complementary");

// All bookings data (except complementary)
const bookingsAll = hotelData.filter((d) => d.MarketSegment != "Complementary");

// Bookings by nationality
const bookingCountry = d3
  .rollups(
    bookingsByMarketSegment,
    (d) => d.length,
    (v) => v.Country
  )
  .map(([name, value]) => ({name, value}))
  .sort((a, b) => d3.descending(a.value, b.value));

// Limit to top 5
const bookingCountryTopN = bookingCountry.slice(0, 5);

// Bin the rest as "Other"
const bookingCountryOther = {
  name: "Other",
  value: d3.sum(bookingCountry.slice(5 - bookingCountry.length), (d) => d.value)
};

// Combine top 5 countries and "other" for donut chart
const byCountry = bookingCountryTopN.concat(bookingCountryOther);

// NEW STUFF: BY LFT MID RHT

const sortedDataP = ministryFile.sort((a, b) => d3.ascending(a.Province, b.Province));

const sortedDataM = ministryFile.sort((a, b) => d3.ascending(a.Province, b.Province));
const mpByProvT = d3.group(sortedDataP, (d) => d.Province, (d) => d.ShortIndustry);

const mpByProvM = d3.group(sortedDataM, (d) => d.Province, (d) => d.IssuedYr, (d) => d.ShortIndustry);
const allByPrvLinechart = d3.group(sortedDataM, (d) => d.Province);


// const fltProvByYr = mpByProvT.
// const specProv = mpByProvT.get("Connecticut");
const sltYear = selectedYr.toString()
const specificProv = mpByProvT.get(pickProvSegment[0]);

const defaultProv = mpByProvM.get("British Columbia");
const defaultPrvName = ["British Columbia"]
const noDefaultPrvName = ["**British Columbia**"]

// LFTCIRCLE STUFF(1/2)
// The province to appear in the middle circle.
let lftText = [pickProvSegment.length < 1 ? defaultPrvName[0] : pickProvSegment[0]];
const lftCircleSpecPrv = mpByProvM.get(
  pickProvSegment.length < 1 ? defaultPrvName[0] : pickProvSegment[0]
);
const lftSeasonBarGraph = allByPrvLinechart.get(lftText[0]);


if (!lftCircleSpecPrv.has(Number(sltYear))){
  lftText = noDefaultPrvName;
}
// The selecting user selected year from lftCircleSpecPrv
const lftSpecPrvYr = [lftCircleSpecPrv.has(Number(sltYear)) ? lftCircleSpecPrv.get(Number(sltYear)) : defaultProv.get(Number(2018))];
// END OF LFTCIRCLE STUFF(1/2)


// MIDCIRCLE STUFF(1/2)
// The province to appear in the middle circle.
let midText = [pickProvSegment.length > 1 ? pickProvSegment[1] : defaultPrvName[0]];

const midSeasonBarGraph = allByPrvLinechart.get(midText[0]);

const midCircleSpecPrv = mpByProvM.get(
  pickProvSegment.length > 1 ? pickProvSegment[1] : defaultPrvName[0]
);
const midLinechart = allByPrvLinechart.get(midText[0]);

if (!midCircleSpecPrv.has(Number(sltYear))){
  midText = noDefaultPrvName;
}
// The selecting user selected year from midCircleSpecPrv
const midSpecPrvYr = [midCircleSpecPrv.has(Number(sltYear)) ? midCircleSpecPrv.get(Number(sltYear)) : defaultProv.get(Number(2018))];
// END OF MIDCIRCLE STUFF(1/2)


// RHTCIRCLE STUFF(1/2)

// The province to appear in the middle circle.
let rhtText = [pickProvSegment.length > 2 ? pickProvSegment[2] : defaultPrvName[0]];

const rhtSeasonBarGraph = allByPrvLinechart.get(rhtText[0]);

const rhtCircleSpecPrv = mpByProvM.get(
  pickProvSegment.length > 2 ? pickProvSegment[2] : defaultPrvName[0]
);
const rhtLinechart = allByPrvLinechart.get(rhtText[0]);

if (!rhtCircleSpecPrv.has(Number(sltYear))){
  rhtText = noDefaultPrvName;
}
// The selecting user selected year from rhtCircleSpecPrv
const rhtSpecPrvYr = [rhtCircleSpecPrv.has(Number(sltYear)) ? rhtCircleSpecPrv.get(Number(sltYear)) : defaultProv.get(Number(2018))];

// END OF RHTCIRCLE STUFF(1/2)

const specIndFL = specificProv.get("COSRV");
const specProvInd = []

const iterator1 = specificProv.values();

for (let i = 0; i < specificProv.size; i++){
  // an array  of 
  // const mapValue = specProvInd.push(iterator1.next().value);
  iterator1.next().value.forEach((element) => specProvInd.push(element))
}

// THE ROLLUPS
const selectedProvIndGrps = d3.rollups(
  specProvInd, 
  (v) => d3.count(v, (d) => d.Amendments), 
  (d) => d.ShortIndustry) 
  .map(([name, value]) => ({name, value}));

// LFTCIRCLE STUFF(2/2)
// lftSpecPrvYr: left cell spec/selected province in a given year
const lftSpecProvYrInd = displayObjA(lftSpecPrvYr[0])
const lftSltdPrvYrIndr = d3.rollups(
  lftSpecProvYrInd, 
  (v) => d3.count(v, (d) => d.Amendments), 
  (d) => d.ShortIndustry) 
  .map(([name, value]) => ({name, value}));

// const lftSltPrvAll = displayObjA(lftCircleSpecPrv)
const lftAllYrs = d3.rollups(lftSeasonBarGraph,
  (v) => d3.sum(v, (d) => d.realValues),
  (d) => d.AwardedYr,
  (d) => d.Season)
  .map(([year, value]) => ({year, value}));

// lftAllYrs.forEach((element) => {element.name = element.name.toString();});
const lftAllYrsNext = transformArray(lftAllYrs)

const midAllYrs = d3.rollups(midSeasonBarGraph,
  (v) => d3.sum(v, (d) => d.realValues),
  (d) => d.AwardedYr,
  (d) => d.Season)
  .map(([year, value]) => ({year, value}));

// midAllYrs.forEach((element) => {element.name = element.name.toString();});
const midAllYrsNext = transformArray(midAllYrs)

const rhtAllYrs = d3.rollups(rhtSeasonBarGraph,
  (v) => d3.sum(v, (d) => d.realValues),
  (d) => d.AwardedYr,
  (d) => d.Season)
  .map(([year, value]) => ({year, value}));

// rhtAllYrs.forEach((element) => {element.name = element.name.toString();});
const rhtAllYrsNext = transformArray(rhtAllYrs)
// .map(([season, amount]) => ({season, amount}))
// const mpByProvT = d3.group(sortedDataP, d => d.Province, (d) => d.ShortIndustry);

const lftSltdPrvYrIndrRev = d3.rollups(
  lftSpecProvYrInd, 
  (v) => d3.sum(v, (d) => d.realValues), 
  (d) => d.ShortIndustry) 
  .map(([name, amount]) => ({name, amount}));
// Sum all industry revenue
const lftSltdPrvYrIndrRevSum = lftSltdPrvYrIndrRev.reduce((n, {amount}) => n + amount, 0);


// LFT TO RENDER
lftSltdPrvYrIndr.sort((a, b) => b.value - a.value);
// Does the slice for top four industries and the rest to other
let lftTorender = topFourInd(lftSltdPrvYrIndr);
// END OF LFTCIRCLE STUFF(2/2)

// MIDCIRCLE STUFF(2/2)
// Returns the values of a map as an array of objects. 
const midSpecProvYrInd = displayObjA(midSpecPrvYr[0])
const midSltdPrvYrIndr = d3.rollups(
  midSpecProvYrInd, 
  (v) => d3.count(v, (d) => d.Amendments), 
  (d) => d.ShortIndustry) 
  .map(([name, value]) => ({name, value}));
// MID TO RENDER
midSltdPrvYrIndr.sort((a, b) => b.value - a.value);
// Does the slice for top four industries and the rest to other
let midTorender = topFourInd(midSltdPrvYrIndr);
const midSltdPrvYrIndrRev = d3.rollups(
  midSpecProvYrInd, 
  (v) => d3.sum(v, (d) => d.realValues), 
  (d) => d.ShortIndustry) 
  .map(([name, amount]) => ({name, amount}));
const midSltdPrvYrIndrRevSum = midSltdPrvYrIndrRev.reduce((n, {amount}) => n + amount, 0);
// END OF MIDCIRCLE STUFF(2/2)

// RHTCIRCLE STUFF(2/2)
const rhtSpecProvYrInd = displayObjA(rhtSpecPrvYr[0])
const rhtSltdPrvYrIndr = d3.rollups(
  rhtSpecProvYrInd, 
  (v) => d3.count(v, (d) => d.Amendments), 
  (d) => d.ShortIndustry) 
  .map(([name, value]) => ({name, value}));

// RHT TO RENDER
rhtSltdPrvYrIndr.sort((a, b) => b.value - a.value);
// Does the slice for top four industries and the rest to other
let rhtTorender = topFourInd(rhtSltdPrvYrIndr);
const rhtSltdPrvYrIndrRev = d3.rollups(
  rhtSpecProvYrInd, 
  (v) => d3.sum(v, (d) => d.realValues), 
  (d) => d.ShortIndustry) 
  .map(([name, amount]) => ({name, amount}));
const rhtSltdPrvYrIndrRevSum = rhtSltdPrvYrIndrRev.reduce((n, {amount}) => n + amount, 0);

// END OF RHTCIRCLE STUFF(2/2)

// END OF THE ROLLUPS

// SORT THE ROLLUPS and pass to fn: topFourInd():
selectedProvIndGrps.sort((a, b) => b.value - a.value);

// END SORT THE ROLLUPS and pass to fn: topFourInd():

let selectedProvByIndTopN = [];
let selectedProvByIndOther = [];
let byIndrusty = [];
// let leftCirlce = topFourInd(selectedProvIndGrps);
// Get 5 indrusties with the highest number of contracts
if (selectedProvIndGrps.length > 4){
  selectedProvByIndTopN = selectedProvIndGrps.slice(0, 4);  
  // Name the rest, other, and sum them
  selectedProvByIndOther = {
    name: "Other",
    value: d3.sum(selectedProvIndGrps.slice(4 - selectedProvIndGrps.length), (d) => d.value)
  };
  // Combine top 5 industry with "other" 
  byIndrusty = selectedProvByIndTopN.concat(selectedProvByIndOther);
} else {
  byIndrusty = selectedProvIndGrps;
}

// SORT THE ROLLUPS


// NEW FUNCTIONS FUNCTION

// Returns the values of a map as an array of objects.
function displayObjA(arrUse){
  let rtArr = []
  const itFn = arrUse.values();
  for (let i = 0; i < arrUse.size; i++){
  // an array  of 
  // const mapValue = specProvInd.push(iterator1.next().value);
  itFn.next().value.forEach((element) => rtArr.push(element))
  }
  return rtArr
}

function transformArray(inputArray) {
  return inputArray.flatMap(({ year, value }) =>
    value.map(([season, amount]) => ({ year, season, amount }))
  );
}



/*The variable topFours must be sorted*/

function topFourInd(topFours){
  /*The variable topFours must be sorted*/
  let slicedTopN = [];
  let nonTopNOther = [];
  let rtTopfour = [];
  // Get 5 indrusties with the highest number of contracts
  if (topFours.length > 4){
    slicedTopN = topFours.slice(0, 3);  
    // Name the rest, other, and sum them
    nonTopNOther = {
      name: "Other",
      value: d3.sum(topFours.slice(3 - topFours.length), (d) => d.value)
    };
    // Combine top 5 industry with "other" 
    rtTopfour = slicedTopN.concat(nonTopNOther);
  } else {
    rtTopfour = topFours;
  }
  return rtTopfour
}
// END OF NEW FUNCTIONS FUNCTION

// END OF NEW STUFF BY LFT MID RHT

//Booking status (cancelled or not cancelled)
const byBookingOutcome = d3
  .rollups(
    bookingsByMarketSegment,
    (d) => d.length,
    (d) => d.IsCanceled
  )
  .map(([name, value]) => ({name, value}))
  .sort((a, b) => d3.descending(a.value, b.value));

// Bookings by room type
const byRoomType = d3
  .rollups(
    bookingsByMarketSegment,
    (d) => d.length,
    (d) => d.ReservedRoomType
  )
  .map(([name, value]) => ({name, value}))
  .sort((a, b) => d3.descending(a.value, b.value));

// Bookings by season
const bookingSeason = d3
  .rollups(
    bookingsByMarketSegment,
    (d) => d.length,
    (v) => v.season
  )
  .map(([name, value]) => ({name, value}));



// Find & format arrival date extent for big number
const arrivalDates = d3.extent(bookingsAll, (d) => d.arrivalDate);
const timeParse = d3.utcParse("%Y-%m-%dT%H");

const newHours = [...new Set(arrivalDates.map(d => d.period))].map(timeParse);
// const [startHour, endHour] = d3.extent(newHours);

const startHour = arrivalDates[0]
const endHour = arrivalDates[1]
const MS_IN_AN_HOUR = 1000 * 60 * 60;

const hrsBackOfData = Math.ceil(Math.abs(endHour - startHour) / (MS_IN_AN_HOUR)) - 1;
const hrsAgoInput = Inputs.range([1, hrsBackOfData], {step: 1, value: 0, width: 150});
const hrsAgo = Generators.input(hrsAgoInput);
hrsAgoInput.querySelector("input[type=number]").remove();

const formatDate = d3.utcFormat("%B %d, %Y");
const currentHour = new Date(endHour.getTime() - hrsAgo * MS_IN_AN_HOUR);
const currentDate = d3.timeFormat("%-d %b %Y")(currentHour);

// const hoursAgoInput = Inputs.range([arrivalDates[0], arrivalDates[1]], {step: 1, value: 0, width: 150});
// const hoursAgo = Generators.input(hoursAgoInput);
// hoursAgoInput.querySelector("input[type=number]").remove();

// const currentHour = new Date(arrivalDates[1].getTime() - hoursAgo * MS_IN_AN_HOUR);
// const currentDate = d3.timeFormat("%b %d, %Y")(new Date(currentHour))

const datesExtent = [
  d3.timeFormat("%b %d, %Y")(new Date(arrivalDates[0])),
  d3.timeFormat("%b %d, %Y")(new Date(arrivalDates[1]))
];



// Calculate rate difference from total average for big number
const rateDiffFromAverage = d3.mean(bookingsByMarketSegment, (d) => d.ADR) - d3.mean(bookingsAll, (d) => d.ADR);
```

```js
// Create search input (for searchable table)
const tableSearch = Inputs.search(bookingsAll);

const tableSearchValue = view(tableSearch);
```

```js
// Line chart (arrival dates)
let mem;
function firstOrRecent(values) {
  return values.length ? (mem = values[0]) : mem;
}
function arrivalLineChart(width, height) {
  return Plot.plot({
    height: height - 50,
    marginBottom: 35,
    width,
    x: {label: "Arrival date"},
    y: {label: "Bookings", grid: true},
    color: {domain: seasonDomain, range: seasonColors, label: "Season"},
    title: `${pickMarketSegment} bookings by arrival date`,
    subtitle: `Daily reservation counts (gray area) and 28-day moving average (solid line).`,
    marks: [
      () => htl.svg`<defs>
      <linearGradient id="gradient" gradientTransform="rotate(90)">
        <stop offset="60%" stop-color="#B5B5B5" stop-opacity="0.7" />
        <stop offset="100%" stop-color="#B5B5B5" stop-opacity="0.1" />
      </linearGradient>
      </defs>`,
      Plot.areaY(
        bookingsByMarketSegment,
        Plot.binX(
          {y: "sum", thresholds: "day", filter: null},
          {
            x: "arrivalDate",
            curve: "step",
            fill: "url(#gradient)"
          }
        )
      ),
      Plot.lineY(
        bookingsByMarketSegment,
        Plot.windowY(
          {k: 28},
          Plot.binX(
            {y: "sum", interval: "day", stroke: firstOrRecent, filter: null},
            {
              x: "arrivalDate",
              strokeWidth: 2,
              stroke: "season",
              z: null,
              tip: {
                format: {
                  arrivalDate: true,
                  bookings: true,
                  x: "%d %b %Y"
                }
              }
            }
          )
        )
      ),
      Plot.ruleY([0]),
      Plot.axisX({ticks: 5}),
      Plot.axisY({ticks: 5})
    ]
  });
}
```

```js
//Create faceted histograms of daily rate

// Season color scheme
const seasonColors = ["#959C00", "#9C5A00", "#465C9C", "#109F73"];
const seasonDomain = ["Summer", "Fall", "Winter", "Spring"];
const yearsDomain = ["2012", "2013", "2014", "2015", "2016", "2017", "2018"]

// Calculate mean daily rate by season (for rule mark)
const meanRateBySeason = d3
  .rollups(
    bookingsByMarketSegment,
    (v) => d3.mean(v, (d) => d.ADR),
    (d) => d.season
  )
  .map(([season, value]) => ({season, value}));

// Build daily rate faceted histograms
const dollarFormat = d3.format("$.2f");
const defaultFormat = d3.format(",");
function dailyRateChart(width, height) {
  return Plot.plot({
    width,
    height: height - 20,
    marginLeft: 30,
    marginRight: 0,
    marginTop: 10,
    marginBottom: 30,
    x: {label: "Average rate($)", grid: true},
    y: {nice: true, label: null},
    axis: null,
    fy: {label: "Season", domain: seasonDomain},
    color: {domain: seasonDomain, range: seasonColors, label: "Season"},
    marks: [
      Plot.axisX({ticks: 4}),
      Plot.axisY({ticks: 2}),
      Plot.rectY(
        bookingsByMarketSegment,
        Plot.binX({y: "count"}, {x: "ADR", interval: 10, fill: "season", fy: "season", tip: true})
      ),
      Plot.text(
        bookingsByMarketSegment,
        Plot.groupZ(
          {text: (v) => `${v[0].season} (n = ${defaultFormat(v.length)})`},
          {
            fy: "season",
            frameAnchor: "top-right",
            dx: -6,
            dy: 6
          }
        )
      ),
      Plot.ruleX(meanRateBySeason, {x: "value", fy: "season", stroke: "currentColor"}),
      Plot.text(meanRateBySeason, {
        x: "value",
        fy: "season",
        text: (d) => `${d.season} mean rate: ${dollarFormat(d.value)}`,
        dx: 5,
        dy: -20,
        textAnchor: "start"
      }),
      Plot.frame({opacity: 0.4})
    ]
  });
}
```

```js
// Faceted bar charts of bookings by room type
// function awardedSeasonChart(width, height) {
//   const numFormat = d3.format("d")
//   return Plot.plot({
//     marginTop: 20,
//     marginBottom: 30,
//     marginLeft: 40,
//     width,
//     height: 270,
//     x: {domain: seasonDomain, tickSize: 0, axis: null, label: "Season"},
//     y: {label: "Count", fontSize: 0, grid: true, insetTop: 5},
//     fx: {label: "Room type"},
//     color: {legend: true, domain: seasonDomain, range: seasonColors, label: "Season"},
//     marks: [
//       // Plot.text({fx: Plot.identity, text: null}),
//       Plot.frame({opacity: 0.4}),
//       Plot.barY(
//         midLinechart,
//         Plot.groupX(
//           {y: "count"},
//           {
//             x: "Season",
//             fx: (d) => `${numFormat(d.AwardedYr)}`,
//             fill: "Season",
//             tip: true
//           }
//         )
//       )
//     ]
//   });
// }
// display(lftAllYrsNext)
// display(lftAllYrs)
// display(keys)
const keys = d3.sort(new Set(lftAllYrsNext.map(d => d.name)));
function lftCircleRevAll(width, province) {
  const numFormatReal = d3.format("d");
  return Plot.plot({
    marginTop: 20,
    marginBottom: 30,
    marginLeft: 40,
    width,
    height: 270,
    x: {axis: null},
    y: {tickFormat: "s", grid: true},
    color: {domain: seasonDomain, 
    range: seasonColors, 
    label: "season",
    legend: true},
    marks: [
      Plot.barY(lftAllYrsNext, {
        x: "season",
        y: "amount",
        fill: "season",
        fx: (d) => `${numFormatReal(d.year)}`,
        sort: {x: null, color: null, fx: {value: "y", reduce: "sum"}},
        tip: true
      }),
      Plot.ruleY([0])
    ]
  })
}


function midCircleRevAll(width, province) {
  const numFormatReal = d3.format("d");
  return Plot.plot({
    marginTop: 20,
    marginBottom: 30,
    marginLeft: 40,
    width,
    height: 270,
    x: {axis: null},
    y: {tickFormat: "s", grid: true},
    color: {domain: seasonDomain, 
    range: seasonColors, 
    label: "season",
    legend: true},
    marks: [
      Plot.barY(midAllYrsNext, {
        x: "season",
        y: "amount",
        fill: "season",
        fx: (d) => `${numFormatReal(d.year)}`,
        sort: {x: null, color: null, fx: {value: "y", reduce: "sum"}},
        tip: true
      }),
      Plot.ruleY([0])
    ]
  })
}

function rhtCircleRevAll(width, province) {
  const numFormatReal = d3.format("d");
  return Plot.plot({
    marginTop: 20,
    marginBottom: 30,
    marginLeft: 40,
    width,
    height: 270,
    x: {axis: null},
    y: {tickFormat: "s", grid: true},
    color: {domain: seasonDomain, 
    range: seasonColors, 
    label: "season",
    legend: true},
    marks: [
      Plot.barY(rhtAllYrsNext, {
        x: "season",
        y: "amount",
        fill: "season",
        fx: (d) => `${numFormatReal(d.year)}`,
        sort: {x: null, color: null, fx: {value: "y", reduce: "sum"}},
        tip: true
      }),
      Plot.ruleY([0])
    ]
  })
}


// function selectedProvRevSumBySeason(width, height) {
//   const numFormatReal = d3.format("d")
//   return Plot.plot({
//     marginTop: 20,
//     marginBottom: 30,
//     marginLeft: 40,
//     width,
//     height: 270,
//     x: {axis: null},
//     y: {tickFormat: "s", grid: true},
//     color: {scheme: "spectral", legend: true},
//     marks: [
//       Plot.barY(lftAllYrsNext, {
//         x: "season",
//         y: "amount",
//         fill: "season",
//         fx: name,
//         sort: {x: null, color: null, fx: {value: "-y", reduce: "sum"}}
//       }),
//       Plot.ruleY([0])
//     ]
//   })
// }
```
