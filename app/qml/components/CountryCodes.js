var name_to_tel = {"Bahrain": "973", "Poland": "48", "French Polynesia": "689", "Malta": "356", "Saudi Arabia": "966", "Madagascar": "261", "Eritrea": "291", "Norway": "47", "Namibia": "264", "Tuvalu": "688", "Ecuador": "593", "Hong Kong": "852", "Antigua & Barbuda": "1268", "Tonga": "676", "Denmark": "45", "New Caledonia": "687", "Norfolk Island": "672", "Chile": "56", "Hungary": "36", "Algeria": "213", "Singapore": "65", "Greenland": "299", "US Virgin Islands": "1340", "Papua New Guinea": "675", "Belize": "501", "Grenada": "1473", "Tokelau": "690", "Cuba": "53", "Sint Maarten": "1721", "Afghanistan": "93", "Cambodia": "855", "Uruguay": "598", "Botswana": "267", "Jordan": "962", "Turkmenistan": "993", "Belarus": "375", "Dominica": "1767", "San Marino": "378", "New Zealand": "64", "Brunei Darussalam": "673", "Georgia": "995", "Lithuania": "370", "Burundi": "257", "Armenia": "374", "Haiti": "509", "Monaco": "377", "Bhutan": "975", "Yemen": "967", "Greece": "30", "Argentina": "54", "Benin": "229", "Czech Republic": "420", "Micronesia": "691", "Nigeria": "234", "Colombia": "57", "Cape Verde": "238", "Belgium": "32", "Congo (Dem. Rep.)": "243", "Turkey": "90", "Switzerland": "41", "Gabon": "241", "Philippines": "63", "Bonaire, Sint Eustatius & Saba": "599", "Solomon Islands": "677", "Latvia": "371", "Mozambique": "258", "Malaysia": "60", "North Korea": "850", "Tunisia": "216", "Costa Rica": "506", "South Korea": "82", "Mali": "223", "France": "33", "Rwanda": "250", "Slovakia": "421", "Israel": "972", "Guam": "1671", "Palau": "680", "Turks & Caicos Islands": "1649", "Myanmar": "95", "Portugal": "351", "Bermuda": "1441", "Croatia": "385", "Egypt": "20", "Tajikistan": "992", "Togo": "228", "Y-land": "42", "Libya": "218", "Gibraltar": "350", "Ethiopia": "251", "Suriname": "597", "Burkina Faso": "226", "Barbados": "1246", "Paraguay": "595", "R\u00e9union": "262", "Iceland": "354", "Saint Lucia": "1758", "Guinea": "224", "Somalia": "252", "Bosnia & Herzegovina": "387", "Italy": "39", "French Guiana": "594", "Malawi": "265", "Ukraine": "380", "Samoa": "685", "Kyrgyzstan": "996", "Uganda": "256", "Macedonia": "389", "United Arab Emirates": "971", "Tanzania": "255", "Ireland": "353", "El Salvador": "503", "Palestine": "970", "United Kingdom": "44", "Honduras": "504", "Guatemala": "502", "Equatorial Guinea": "240", "Saint Pierre & Miquelon": "508", "Sweden": "46", "Diego Garcia": "246", "Spain": "34", "Bahamas": "1242", "Saint Vincent & the Grenadines": "1784", "Syrian Arab Republic": "963", "Guadeloupe": "590", "Qatar": "974", "Vietnam": "84", "Puerto Rico": "1", "Azerbaijan": "994", "Zambia": "260", "Niger": "227", "China": "86", "Taiwan": "886", "Cyprus": "357", "Wallis & Futuna": "681", "Thailand": "66", "Trinidad & Tobago": "1868", "Iran": "98", "Congo (Rep.)": "242", "Comoros": "269", "Anguilla": "1264", "Ghana": "233", "Martinique": "596", "Guyana": "592", "Serbia": "381", "Pakistan": "92", "Morocco": "212", "Germany": "49", "Northern Mariana Islands": "1670", "South Africa": "27", "Chad": "235", "Finland": "358", "Cameroon": "237", "Aruba": "297", "Lesotho": "266", "Andorra": "376", "Swaziland": "268", "C\u00f4te d`Ivoire": "225", "Austria": "43", "Falkland Islands": "500", "Kenya": "254", "Peru": "51", "Central African Rep.": "236", "Nauru": "674", "Niue": "683", "Indonesia": "62", "Cook Islands": "682", "Sudan": "249", "Nicaragua": "505", "Albania": "355", "Canada": "1", "Luxembourg": "352", "India": "91", "S\u00e3o Tom\u00e9 & Pr\u00edncipe": "239", "Timor-Leste": "670", "Uzbekistan": "998", "Montenegro": "382", "Macau": "853", "Romania": "40", "Bolivia": "591", "Sri Lanka": "94", "Montserrat": "1664", "Jamaica": "1876", "Saint Kitts & Nevis": "1869", "Guinea-Bissau": "245", "Kiribati": "686", "British Virgin Islands": "1284", "Mexico": "52", "Cayman Islands": "1345", "Kazakhstan": "7", "Mongolia": "976", "Seychelles": "248", "Nepal": "977", "Mauritania": "222", "Australia": "61", "Japan": "81", "Marshall Islands": "692", "Dominican Rep.": "1", "Russian Federation": "7", "Venezuela": "58", "Panama": "507", "Lebanon": "961", "Mauritius": "230", "Liberia": "231", "Bulgaria": "359", "Oman": "968", "Vanuatu": "678", "Netherlands": "31", "Maldives": "960", "Iraq": "964", "Zimbabwe": "263", "Bangladesh": "880", "USA": "1", "Fiji": "679", "Sierra Leone": "232", "Gambia": "220", "Kuwait": "965", "Senegal": "221", "Cura\u00e7ao": "599", "Saint Helena": "247", "Estonia": "372", "Faroe Islands": "298", "Slovenia": "386", "Moldova": "373", "Djibouti": "253", "American Samoa": "1684", "Laos": "856", "Liechtenstein": "423", "Angola": "244", "South Sudan": "211", "Brazil": "55"};
var name_to_iso = {"Bahrain": "BH", "Poland": "PL", "French Polynesia": "PF", "Malta": "MT", "Saudi Arabia": "SA", "Madagascar": "MG", "Eritrea": "ER", "Norway": "NO", "Namibia": "NA", "Tuvalu": "TV", "Ecuador": "EC", "Hong Kong": "HK", "Antigua & Barbuda": "AG", "Tonga": "TO", "Denmark": "DK", "New Caledonia": "NC", "Norfolk Island": "NF", "Chile": "CL", "Hungary": "HU", "Algeria": "DZ", "Singapore": "SG", "Greenland": "GL", "US Virgin Islands": "VI", "Papua New Guinea": "PG", "Belize": "BZ", "Grenada": "GD", "Tokelau": "TK", "Cuba": "CU", "Sint Maarten": "SX", "Afghanistan": "AF", "Cambodia": "KH", "Uruguay": "UY", "Botswana": "BW", "Jordan": "JO", "Turkmenistan": "TM", "Belarus": "BY", "Dominica": "DM", "San Marino": "SM", "New Zealand": "NZ", "Brunei Darussalam": "BN", "Georgia": "GE", "Lithuania": "LT", "Burundi": "BI", "Armenia": "AM", "Haiti": "HT", "Monaco": "MC", "Bhutan": "BT", "Yemen": "YE", "Greece": "GR", "Argentina": "AR", "Benin": "BJ", "Czech Republic": "CZ", "Micronesia": "FM", "Nigeria": "NG", "Colombia": "CO", "Cape Verde": "CV", "Belgium": "BE", "Congo (Dem. Rep.)": "CD", "Turkey": "TR", "Switzerland": "CH", "Gabon": "GA", "Philippines": "PH", "Bonaire, Sint Eustatius & Saba": "BQ", "Solomon Islands": "SB", "Latvia": "LV", "Mozambique": "MZ", "Malaysia": "MY", "North Korea": "KP", "Tunisia": "TN", "Costa Rica": "CR", "South Korea": "KR", "Mali": "ML", "France": "FR", "Rwanda": "RW", "Slovakia": "SK", "Israel": "IL", "Guam": "GU", "Palau": "PW", "Turks & Caicos Islands": "TC", "Myanmar": "MM", "Portugal": "PT", "Bermuda": "BM", "Croatia": "HR", "Egypt": "EG", "Tajikistan": "TJ", "Togo": "TG", "Y-land": "YL", "Libya": "LY", "Gibraltar": "GI", "Ethiopia": "ET", "Suriname": "SR", "Burkina Faso": "BF", "Barbados": "BB", "Paraguay": "PY", "R\u00e9union": "RE", "Iceland": "IS", "Saint Lucia": "LC", "Guinea": "GN", "Somalia": "SO", "Bosnia & Herzegovina": "BA", "Italy": "IT", "French Guiana": "GF", "Malawi": "MW", "Ukraine": "UA", "Samoa": "WS", "Kyrgyzstan": "KG", "Uganda": "UG", "Macedonia": "MK", "United Arab Emirates": "AE", "Tanzania": "TZ", "Ireland": "IE", "El Salvador": "SV", "Palestine": "PS", "United Kingdom": "GB", "Honduras": "HN", "Guatemala": "GT", "Equatorial Guinea": "GQ", "Saint Pierre & Miquelon": "PM", "Sweden": "SE", "Diego Garcia": "IO", "Spain": "ES", "Bahamas": "BS", "Saint Vincent & the Grenadines": "VC", "Syrian Arab Republic": "SY", "Guadeloupe": "GP", "Qatar": "QA", "Vietnam": "VN", "Puerto Rico": "PR", "Azerbaijan": "AZ", "Zambia": "ZM", "Niger": "NE", "China": "CN", "Taiwan": "TW", "Cyprus": "CY", "Wallis & Futuna": "WF", "Thailand": "TH", "Trinidad & Tobago": "TT", "Iran": "IR", "Congo (Rep.)": "CG", "Comoros": "KM", "Anguilla": "AI", "Ghana": "GH", "Martinique": "MQ", "Guyana": "GY", "Serbia": "RS", "Pakistan": "PK", "Morocco": "MA", "Germany": "DE", "Northern Mariana Islands": "MP", "South Africa": "ZA", "Chad": "TD", "Finland": "FI", "Cameroon": "CM", "Aruba": "AW", "Lesotho": "LS", "Andorra": "AD", "Swaziland": "SZ", "C\u00f4te d`Ivoire": "CI", "Austria": "AT", "Falkland Islands": "FK", "Kenya": "KE", "Peru": "PE", "Central African Rep.": "CF", "Nauru": "NR", "Niue": "NU", "Indonesia": "ID", "Cook Islands": "CK", "Sudan": "SD", "Nicaragua": "NI", "Albania": "AL", "Canada": "CA", "Luxembourg": "LU", "India": "IN", "S\u00e3o Tom\u00e9 & Pr\u00edncipe": "ST", "Timor-Leste": "TL", "Uzbekistan": "UZ", "Montenegro": "ME", "Macau": "MO", "Romania": "RO", "Bolivia": "BO", "Sri Lanka": "LK", "Montserrat": "MS", "Jamaica": "JM", "Saint Kitts & Nevis": "KN", "Guinea-Bissau": "GW", "Kiribati": "KI", "British Virgin Islands": "VG", "Mexico": "MX", "Cayman Islands": "KY", "Kazakhstan": "KZ", "Mongolia": "MN", "Seychelles": "SC", "Nepal": "NP", "Mauritania": "MR", "Australia": "AU", "Japan": "JP", "Marshall Islands": "MH", "Dominican Rep.": "DO", "Russian Federation": "RU", "Venezuela": "VE", "Panama": "PA", "Lebanon": "LB", "Mauritius": "MU", "Liberia": "LR", "Bulgaria": "BG", "Oman": "OM", "Vanuatu": "VU", "Netherlands": "NL", "Maldives": "MV", "Iraq": "IQ", "Zimbabwe": "ZW", "Bangladesh": "BD", "USA": "US", "Fiji": "FJ", "Sierra Leone": "SL", "Gambia": "GM", "Kuwait": "KW", "Senegal": "SN", "Cura\u00e7ao": "CW", "Saint Helena": "SH", "Estonia": "EE", "Faroe Islands": "FO", "Slovenia": "SI", "Moldova": "MD", "Djibouti": "DJ", "American Samoa": "AS", "Laos": "LA", "Liechtenstein": "LI", "Angola": "AO", "South Sudan": "SS", "Brazil": "BR"};
var iso_to_name = {"SI": "Slovenia", "BG": "Bulgaria", "NZ": "New Zealand", "AD": "Andorra", "NP": "Nepal", "LA": "Laos", "GT": "Guatemala", "BZ": "Belize", "UZ": "Uzbekistan", "UY": "Uruguay", "MQ": "Martinique", "BN": "Brunei Darussalam", "MM": "Myanmar", "EE": "Estonia", "ML": "Mali", "SB": "Solomon Islands", "MD": "Moldova", "CO": "Colombia", "LB": "Lebanon", "BF": "Burkina Faso", "JP": "Japan", "PW": "Palau", "MA": "Morocco", "LI": "Liechtenstein", "OM": "Oman", "KW": "Kuwait", "AE": "United Arab Emirates", "AS": "American Samoa", "CA": "Canada", "MP": "Northern Mariana Islands", "KH": "Cambodia", "BQ": "Bonaire, Sint Eustatius & Saba", "MX": "Mexico", "TV": "Tuvalu", "HU": "Hungary", "AL": "Albania", "KM": "Comoros", "CR": "Costa Rica", "RU": "Russian Federation", "CL": "Chile", "DZ": "Algeria", "QA": "Qatar", "DM": "Dominica", "PY": "Paraguay", "PA": "Panama", "AU": "Australia", "VN": "Vietnam", "PL": "Poland", "GU": "Guam", "TG": "Togo", "SL": "Sierra Leone", "BY": "Belarus", "PS": "Palestine", "CN": "China", "AG": "Antigua & Barbuda", "CM": "Cameroon", "NA": "Namibia", "MT": "Malta", "CW": "Cura\u00e7ao", "LC": "Saint Lucia", "MY": "Malaysia", "DE": "Germany", "DK": "Denmark", "LR": "Liberia", "GY": "Guyana", "SK": "Slovakia", "LU": "Luxembourg", "TZ": "Tanzania", "BW": "Botswana", "GF": "French Guiana", "YE": "Yemen", "LK": "Sri Lanka", "SM": "San Marino", "IS": "Iceland", "HN": "Honduras", "AO": "Angola", "TO": "Tonga", "SN": "Senegal", "FJ": "Fiji", "ZA": "South Africa", "MV": "Maldives", "GR": "Greece", "SD": "Sudan", "TM": "Turkmenistan", "GM": "Gambia", "CG": "Congo (Rep.)", "GN": "Guinea", "NO": "Norway", "RS": "Serbia", "TW": "Taiwan", "RE": "R\u00e9union", "MZ": "Mozambique", "EC": "Ecuador", "SH": "Saint Helena", "FR": "France", "PM": "Saint Pierre & Miquelon", "GH": "Ghana", "FK": "Falkland Islands", "CV": "Cape Verde", "TL": "Timor-Leste", "TD": "Chad", "FM": "Micronesia", "AW": "Aruba", "NE": "Niger", "BH": "Bahrain", "IR": "Iran", "IE": "Ireland", "ID": "Indonesia", "CD": "Congo (Dem. Rep.)", "BS": "Bahamas", "GA": "Gabon", "BT": "Bhutan", "CU": "Cuba", "FO": "Faroe Islands", "NR": "Nauru", "LV": "Latvia", "TJ": "Tajikistan", "TC": "Turks & Caicos Islands", "HK": "Hong Kong", "HR": "Croatia", "CI": "C\u00f4te d`Ivoire", "KR": "South Korea", "MC": "Monaco", "VI": "US Virgin Islands", "AT": "Austria", "RW": "Rwanda", "WS": "Samoa", "ME": "Montenegro", "UG": "Uganda", "JM": "Jamaica", "ST": "S\u00e3o Tom\u00e9 & Pr\u00edncipe", "US": "USA", "CZ": "Czech Republic", "PR": "Puerto Rico", "PK": "Pakistan", "KZ": "Kazakhstan", "YL": "Y-land", "VG": "British Virgin Islands", "IO": "Diego Garcia", "AF": "Afghanistan", "IL": "Israel", "TN": "Tunisia", "CF": "Central African Rep.", "SY": "Syrian Arab Republic", "GP": "Guadeloupe", "AZ": "Azerbaijan", "CK": "Cook Islands", "MR": "Mauritania", "EG": "Egypt", "PG": "Papua New Guinea", "JO": "Jordan", "ZM": "Zambia", "LY": "Libya", "BI": "Burundi", "SC": "Seychelles", "SO": "Somalia", "BR": "Brazil", "KG": "Kyrgyzstan", "TH": "Thailand", "MO": "Macau", "UA": "Ukraine", "TK": "Tokelau", "SX": "Sint Maarten", "BB": "Barbados", "NF": "Norfolk Island", "BE": "Belgium", "GI": "Gibraltar", "MH": "Marshall Islands", "ZW": "Zimbabwe", "ES": "Spain", "IT": "Italy", "BO": "Bolivia", "VU": "Vanuatu", "PH": "Philippines", "BD": "Bangladesh", "TR": "Turkey", "MU": "Mauritius", "IN": "India", "SS": "South Sudan", "HT": "Haiti", "MW": "Malawi", "MS": "Montserrat", "DJ": "Djibouti", "KN": "Saint Kitts & Nevis", "GW": "Guinea-Bissau", "KE": "Kenya", "FI": "Finland", "SV": "El Salvador", "MG": "Madagascar", "AM": "Armenia", "IQ": "Iraq", "SA": "Saudi Arabia", "VE": "Venezuela", "NC": "New Caledonia", "PT": "Portugal", "LT": "Lithuania", "CH": "Switzerland", "GE": "Georgia", "SG": "Singapore", "CY": "Cyprus", "MK": "Macedonia", "RO": "Romania", "AI": "Anguilla", "SR": "Suriname", "AR": "Argentina", "SE": "Sweden", "PF": "French Polynesia", "GL": "Greenland", "GQ": "Equatorial Guinea", "PE": "Peru", "BM": "Bermuda", "LS": "Lesotho", "NI": "Nicaragua", "DO": "Dominican Rep.", "ET": "Ethiopia", "BJ": "Benin", "NL": "Netherlands", "KY": "Cayman Islands", "TT": "Trinidad & Tobago", "ER": "Eritrea", "NG": "Nigeria", "WF": "Wallis & Futuna", "SZ": "Swaziland", "KI": "Kiribati", "BA": "Bosnia & Herzegovina", "KP": "North Korea", "GD": "Grenada", "MN": "Mongolia", "GB": "United Kingdom", "NU": "Niue", "VC": "Saint Vincent & the Grenadines"};
var iso_to_tel = {"SI": "386", "BG": "359", "NZ": "64", "AD": "376", "NP": "977", "LA": "856", "GT": "502", "BZ": "501", "UZ": "998", "UY": "598", "MQ": "596", "BN": "673", "MM": "95", "EE": "372", "ML": "223", "SB": "677", "MD": "373", "CO": "57", "LB": "961", "BF": "226", "JP": "81", "PW": "680", "MA": "212", "LI": "423", "OM": "968", "KW": "965", "AE": "971", "AS": "1684", "CA": "1", "MP": "1670", "KH": "855", "BQ": "599", "MX": "52", "TV": "688", "HU": "36", "AL": "355", "KM": "269", "CR": "506", "RU": "7", "CL": "56", "DZ": "213", "QA": "974", "DM": "1767", "PY": "595", "PA": "507", "AU": "61", "VN": "84", "PL": "48", "GU": "1671", "TG": "228", "SL": "232", "BY": "375", "PS": "970", "CN": "86", "AG": "1268", "CM": "237", "NA": "264", "MT": "356", "CW": "599", "LC": "1758", "MY": "60", "DE": "49", "DK": "45", "LR": "231", "GY": "592", "SK": "421", "LU": "352", "TZ": "255", "BW": "267", "GF": "594", "YE": "967", "LK": "94", "SM": "378", "IS": "354", "HN": "504", "AO": "244", "TO": "676", "SN": "221", "FJ": "679", "ZA": "27", "MV": "960", "GR": "30", "SD": "249", "TM": "993", "GM": "220", "CG": "242", "GN": "224", "NO": "47", "RS": "381", "TW": "886", "RE": "262", "MZ": "258", "EC": "593", "SH": "247", "FR": "33", "PM": "508", "GH": "233", "FK": "500", "CV": "238", "TL": "670", "TD": "235", "FM": "691", "AW": "297", "NE": "227", "BH": "973", "IR": "98", "IE": "353", "ID": "62", "CD": "243", "BS": "1242", "GA": "241", "BT": "975", "CU": "53", "FO": "298", "NR": "674", "LV": "371", "TJ": "992", "TC": "1649", "HK": "852", "HR": "385", "CI": "225", "KR": "82", "MC": "377", "VI": "1340", "AT": "43", "RW": "250", "WS": "685", "ME": "382", "UG": "256", "JM": "1876", "ST": "239", "US": "1", "CZ": "420", "PR": "1", "PK": "92", "KZ": "7", "YL": "42", "VG": "1284", "IO": "246", "AF": "93", "IL": "972", "TN": "216", "CF": "236", "SY": "963", "GP": "590", "AZ": "994", "CK": "682", "MR": "222", "EG": "20", "PG": "675", "JO": "962", "ZM": "260", "LY": "218", "BI": "257", "SC": "248", "SO": "252", "BR": "55", "KG": "996", "TH": "66", "MO": "853", "UA": "380", "TK": "690", "SX": "1721", "BB": "1246", "NF": "672", "BE": "32", "GI": "350", "MH": "692", "ZW": "263", "ES": "34", "IT": "39", "BO": "591", "VU": "678", "PH": "63", "BD": "880", "TR": "90", "MU": "230", "IN": "91", "SS": "211", "HT": "509", "MW": "265", "MS": "1664", "DJ": "253", "KN": "1869", "GW": "245", "KE": "254", "FI": "358", "SV": "503", "MG": "261", "AM": "374", "IQ": "964", "SA": "966", "VE": "58", "NC": "687", "PT": "351", "LT": "370", "CH": "41", "GE": "995", "SG": "65", "CY": "357", "MK": "389", "RO": "40", "AI": "1264", "SR": "597", "AR": "54", "SE": "46", "PF": "689", "GL": "299", "GQ": "240", "PE": "51", "BM": "1441", "LS": "266", "NI": "505", "DO": "1", "ET": "251", "BJ": "229", "NL": "31", "KY": "1345", "TT": "1868", "ER": "291", "NG": "234", "WF": "681", "SZ": "268", "KI": "686", "BA": "387", "KP": "850", "GD": "1473", "MN": "976", "GB": "44", "NU": "683", "VC": "1784"};
var tel_to_name = {"591": "Bolivia", "211": "South Sudan", "1246": "Barbados", "386": "Slovenia", "238": "Cape Verde", "966": "Saudi Arabia", "358": "Finland", "1869": "Saint Kitts & Nevis", "1876": "Jamaica", "375": "Belarus", "972": "Israel", "674": "Nauru", "681": "Wallis & Futuna", "995": "Georgia", "689": "French Polynesia", "1268": "Antigua & Barbuda", "226": "Burkina Faso", "258": "Mozambique", "503": "El Salvador", "595": "Paraguay", "994": "Azerbaijan", "57": "Colombia", "223": "Mali", "500": "Falkland Islands", "239": "S\u00e3o Tom\u00e9 & Pr\u00edncipe", "51": "Peru", "261": "Madagascar", "992": "Tajikistan", "222": "Mauritania", "52": "Mexico", "378": "San Marino", "34": "Spain", "1868": "Trinidad & Tobago", "590": "Guadeloupe", "267": "Botswana", "1758": "Saint Lucia", "60": "Malaysia", "886": "Taiwan", "682": "Cook Islands", "7": "Russian Federation", "357": "Cyprus", "262": "R\u00e9union", "65": "Singapore", "597": "Suriname", "371": "Latvia", "963": "Syrian Arab Republic", "236": "Central African Rep.", "216": "Tunisia", "250": "Rwanda", "260": "Zambia", "676": "Tonga", "46": "Sweden", "974": "Qatar", "237": "Cameroon", "224": "Guinea", "678": "Vanuatu", "56": "Chile", "39": "Italy", "507": "Panama", "91": "India", "40": "Romania", "855": "Cambodia", "856": "Laos", "688": "Tuvalu", "49": "Germany", "996": "Kyrgyzstan", "266": "Lesotho", "683": "Niue", "370": "Lithuania", "1345": "Cayman Islands", "230": "Mauritius", "387": "Bosnia & Herzegovina", "43": "Austria", "55": "Brazil", "599": "Cura\u00e7ao", "233": "Ghana", "82": "South Korea", "62": "Indonesia", "355": "Albania", "380": "Ukraine", "352": "Luxembourg", "1473": "Grenada", "509": "Haiti", "350": "Gibraltar", "692": "Marshall Islands", "382": "Montenegro", "506": "Costa Rica", "44": "United Kingdom", "675": "Papua New Guinea", "1721": "Sint Maarten", "299": "Greenland", "691": "Micronesia", "242": "Congo (Rep.)", "1649": "Turks & Caicos Islands", "255": "Tanzania", "356": "Malta", "213": "Algeria", "1670": "Northern Mariana Islands", "54": "Argentina", "973": "Bahrain", "502": "Guatemala", "504": "Honduras", "880": "Bangladesh", "98": "Iran", "971": "United Arab Emirates", "964": "Iraq", "1441": "Bermuda", "679": "Fiji", "354": "Iceland", "220": "Gambia", "677": "Solomon Islands", "1264": "Anguilla", "232": "Sierra Leone", "687": "New Caledonia", "420": "Czech Republic", "53": "Cuba", "592": "Guyana", "227": "Niger", "47": "Norway", "975": "Bhutan", "264": "Namibia", "961": "Lebanon", "269": "Comoros", "231": "Liberia", "27": "South Africa", "263": "Zimbabwe", "291": "Eritrea", "297": "Aruba", "673": "Brunei Darussalam", "268": "Swaziland", "968": "Oman", "381": "Serbia", "685": "Samoa", "962": "Jordan", "31": "Netherlands", "247": "Saint Helena", "228": "Togo", "32": "Belgium", "90": "Turkey", "64": "New Zealand", "241": "Gabon", "1767": "Dominica", "998": "Uzbekistan", "42": "Y-land", "84": "Vietnam", "33": "France", "41": "Switzerland", "1684": "American Samoa", "221": "Senegal", "95": "Myanmar", "86": "China", "421": "Slovakia", "960": "Maldives", "92": "Pakistan", "376": "Andorra", "1284": "British Virgin Islands", "256": "Uganda", "672": "Norfolk Island", "686": "Kiribati", "680": "Palau", "1671": "Guam", "225": "C\u00f4te d`Ivoire", "48": "Poland", "45": "Denmark", "245": "Guinea-Bissau", "1340": "US Virgin Islands", "353": "Ireland", "1242": "Bahamas", "66": "Thailand", "965": "Kuwait", "423": "Liechtenstein", "594": "French Guiana", "389": "Macedonia", "976": "Mongolia", "235": "Chad", "290": "Saint Helena", "252": "Somalia", "244": "Angola", "240": "Equatorial Guinea", "63": "Philippines", "61": "Australia", "690": "Tokelau", "508": "Saint Pierre & Miquelon", "58": "Venezuela", "94": "Sri Lanka", "596": "Martinique", "853": "Macau", "351": "Portugal", "977": "Nepal", "243": "Congo (Dem. Rep.)", "254": "Kenya", "593": "Ecuador", "298": "Faroe Islands", "249": "Sudan", "234": "Nigeria", "212": "Morocco", "377": "Monaco", "970": "Palestine", "967": "Yemen", "251": "Ethiopia", "850": "North Korea", "218": "Libya", "598": "Uruguay", "385": "Croatia", "248": "Seychelles", "373": "Moldova", "1": "USA", "93": "Afghanistan", "257": "Burundi", "265": "Malawi", "253": "Djibouti", "359": "Bulgaria", "372": "Estonia", "20": "Egypt", "993": "Turkmenistan", "505": "Nicaragua", "246": "Diego Garcia", "501": "Belize", "1784": "Saint Vincent & the Grenadines", "852": "Hong Kong", "30": "Greece", "374": "Armenia", "1664": "Montserrat", "81": "Japan", "229": "Benin", "36": "Hungary", "670": "Timor-Leste"};
var tel_to_iso = {"591": "BO", "211": "SS", "1246": "BB", "386": "SI", "238": "CV", "966": "SA", "358": "FI", "1869": "KN", "1876": "JM", "375": "BY", "972": "IL", "674": "NR", "681": "WF", "995": "GE", "689": "PF", "1268": "AG", "226": "BF", "258": "MZ", "503": "SV", "595": "PY", "994": "AZ", "57": "CO", "223": "ML", "500": "FK", "239": "ST", "51": "PE", "261": "MG", "992": "TJ", "222": "MR", "52": "MX", "378": "SM", "34": "ES", "1868": "TT", "590": "GP", "267": "BW", "1758": "LC", "60": "MY", "886": "TW", "682": "CK", "7": "RU", "357": "CY", "262": "RE", "65": "SG", "597": "SR", "371": "LV", "963": "SY", "236": "CF", "216": "TN", "250": "RW", "260": "ZM", "676": "TO", "46": "SE", "974": "QA", "237": "CM", "224": "GN", "678": "VU", "56": "CL", "39": "IT", "507": "PA", "91": "IN", "40": "RO", "855": "KH", "856": "LA", "688": "TV", "49": "DE", "996": "KG", "266": "LS", "683": "NU", "370": "LT", "1345": "KY", "230": "MU", "387": "BA", "43": "AT", "55": "BR", "599": "CW", "233": "GH", "82": "KR", "62": "ID", "355": "AL", "380": "UA", "352": "LU", "1473": "GD", "509": "HT", "350": "GI", "692": "MH", "382": "ME", "506": "CR", "44": "GB", "675": "PG", "1721": "SX", "299": "GL", "691": "FM", "242": "CG", "1649": "TC", "255": "TZ", "356": "MT", "213": "DZ", "1670": "MP", "54": "AR", "973": "BH", "502": "GT", "504": "HN", "880": "BD", "98": "IR", "971": "AE", "964": "IQ", "1441": "BM", "679": "FJ", "354": "IS", "220": "GM", "677": "SB", "1264": "AI", "232": "SL", "687": "NC", "420": "CZ", "53": "CU", "592": "GY", "227": "NE", "47": "NO", "975": "BT", "264": "NA", "961": "LB", "269": "KM", "231": "LR", "27": "ZA", "263": "ZW", "291": "ER", "297": "AW", "673": "BN", "268": "SZ", "968": "OM", "381": "RS", "685": "WS", "962": "JO", "31": "NL", "247": "SH", "228": "TG", "32": "BE", "90": "TR", "64": "NZ", "241": "GA", "1767": "DM", "998": "UZ", "42": "YL", "84": "VN", "33": "FR", "41": "CH", "1684": "AS", "221": "SN", "95": "MM", "86": "CN", "421": "SK", "960": "MV", "92": "PK", "376": "AD", "1284": "VG", "256": "UG", "672": "NF", "686": "KI", "680": "PW", "1671": "GU", "225": "CI", "48": "PL", "45": "DK", "245": "GW", "1340": "VI", "353": "IE", "1242": "BS", "66": "TH", "965": "KW", "423": "LI", "594": "GF", "389": "MK", "976": "MN", "235": "TD", "290": "SH", "252": "SO", "244": "AO", "240": "GQ", "63": "PH", "61": "AU", "690": "TK", "508": "PM", "58": "VE", "94": "LK", "596": "MQ", "853": "MO", "351": "PT", "977": "NP", "243": "CD", "254": "KE", "593": "EC", "298": "FO", "249": "SD", "234": "NG", "212": "MA", "377": "MC", "970": "PS", "967": "YE", "251": "ET", "850": "KP", "218": "LY", "598": "UY", "385": "HR", "248": "SC", "373": "MD", "1": "US", "93": "AF", "257": "BI", "265": "MW", "253": "DJ", "359": "BG", "372": "EE", "20": "EG", "993": "TM", "505": "NI", "246": "IO", "501": "BZ", "1784": "VC", "852": "HK", "30": "GR", "374": "AM", "1664": "MS", "81": "JP", "229": "BJ", "36": "HU", "670": "TL"}
