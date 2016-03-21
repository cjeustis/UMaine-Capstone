var mrPour = angular.module('mrPour', ['ngAnimate', 'ngRoute', 'LocalStorageModule', 'ui.bootstrap', 'agGrid', 'ngMessages']);

mrPour.config(function ($routeProvider, localStorageServiceProvider) {

    localStorageServiceProvider
        .setPrefix('mrPour')
        .setStorageType('localStorage')
        .setNotify(true, true);


    $routeProvider
    .when('/index.html', {
        templateUrl: 'html/main.html',
        controller: 'mainController',
        resolve: function($q, $location) {
            var deferred = $q.defer();
            deferred.resolve();
            if (!localStorageService.get('auth')) {
               $location.path('/login');
            }
            return deferred.promise;
        }
    })

    .when('/registration', {
         templateUrl: 'html/registration-page.html',
         controller: 'registrationController'
    })

    .when('/login', {
         templateUrl: 'html/login-page.html',
         controller: 'loginController'
    })

    .when('/dashboard', {
        templateUrl: 'html/dash.html',
        controller: 'dashboardController'
    })

    .when('/dashboard/recipes', {
        templateUrl: 'html/recipes.html',
        controller: 'recipeController'
    })

    .when('/dashboard/recipes/create', {
        templateUrl: 'html/createRecipe.html',
        controller: 'createController'
    })

    .when('/dashboard/recipes/update', {
        templateUrl: 'html/updateRecipe.html',
        controller: 'updateController'
    })

    .when('/dashboard/temperature', {
        templateUrl: 'html/temp.html',
        controller: 'tempController'
    })

    .otherwise({
        redirectTo: '/index.html'
    });

    // use the HTML5 History API
    //$locationProvider.html5Mode(true);
});

mrPour.run(function($rootScope, localStorageService, $location) {
    var urlPath = window.location.href;

    $('#loading').hide();
    $rootScope.isBusy = false;

    setInterval(function() {
        // Only read and send temp if doing nothing else
        if (!$rootScope.isBusy) {
            $rootScope.isBusy = true;

            // read temp value
            $.ajax({
                type: 'post',
                url: 'php/getTemp.php',
                success: function (data) {
                    var d = JSON.parse(data);
                    $rootScope.temp['temp'] = ((d.Temp * 9) / 5 + 32);
                }
            });

            while ($rootScope.temp['temp'] == 32) {
                // read temp value again
                $.ajax({
                    type: 'post',
                    url: 'php/getTemp.php',
                    success: function (data) {
                        var d = JSON.parse(data);
                        $rootScope.temp['temp'] = ((d.Temp * 9) / 5 + 32);
                    }
                });
            }

            // send temp value to avr
            $.ajax({
                type: 'post',
                url: 'php/sendTempData.php',
                data: $rootScope.temp,
                success: function (data) {
                    console.log("Temp read: " + $rootScope.temp['temp']);
                    console.log("Sent temp to avr");
                }
            });

            $rootScope.isBusy = false;
        }
    }, 5000);

    /* Inactivty timer to remove authentication */
    var t;
    if (localStorageService.get('auth')) {
        window.onload = resetTimer;
        document.onmousemove = resetTimer;
        document.onkeypress = resetTimer;
    }

    $rootScope.temp = {};
    $rootScope.temp['control'] = 'c';

    function logout() {
        localStorageService.set('auth', false);
        $location.path('/login');
        $rootScope.$apply();
    }

    function resetTimer() {
        clearTimeout(t);
        var now = Date.now();
        var expires = now + 3600000;
        localStorageService.set('expires', expires);
        t = setTimeout(logout, 1200000);    // 20 minutes for inactivity
    }
    /* End of inactivity timer logic */

    $rootScope.$on('$locationChangeStart', function() {
        $rootScope.isAuth = localStorageService.get('auth');
        // not authorized
        if (!localStorageService.get('auth')) {
            if(urlPath.indexOf("dashboard") > -1) {
                $location.path('/login');
            }
        }
        // authorized
        else {
            var expires = localStorageService.get('expires');
            // if token is expired
            if (expires <= Date.now()) {
                $rootScope.expires = expires;
                $rootScope.isAuth = false;
                localStorageService.set('auth', false);
                $location.path('/login');
            }
            else {
                $.getJSON('php/session.php', function(data) {
                    $rootScope.$apply(function() {
                        $rootScope.userData = data;
                    });
                }).error(function(data) { console.log(data); });

                if(urlPath.indexOf("index") > -1) {
                    $location.path('/dashboard');
                }
            }
        }
    });
});

mrPour.directive('modalDialog', function() {
    return {
        restrict: 'E',
        scope: {
            show: '='
        },
        replace: true, // Replace with the template below
        transclude: true, // we want to insert custom content inside the directive
        link: function(scope, element, attrs) {
            scope.dialogStyle = {};
            if (attrs.width)
                scope.dialogStyle.width = attrs.width;
            if (attrs.height)
                scope.dialogStyle.height = attrs.height;
            scope.hideModal = function() {
                scope.show = false;
            };
        },
        templateUrl: "../html/modal.html"
    };
});

//mrPour.factory('MathService', function() {
//    var factory = {};
//
//    factory.multiply = function(a, b) {
//        return a * b
//    };
//
//    return factory;
//});

// create the controller and inject Angular's $scope
mrPour.controller('mainController', function ($scope, $rootScope, $location, $anchorScroll) {
    $scope.scrollTo = function(id) {
        $location.hash(id);
        $anchorScroll();
    };
});

mrPour.controller('registrationController', function ($scope, $rootScope, localStorageService, $location) {

    if (localStorageService.get('auth')) {
        $location.path('/dashboard');
        $scope.$apply();
    }

    var regFormData = {};

    $('#register-form').validate({
        rules:
        {
            user_name: {
                required: true,
                minlength: 3
            },
            password: {
                required: true,
                minlength: 8,
                maxlength: 15
            },
            cpassword: {
                required: true,
                equalTo: '#password'
            },
            user_email: {
                required: true,
                email: true
            }
        },
        messages:
        {
            user_name: "Username required.",
            password:{
                required: "Password required.",
                minlength: "Password must be at least 8 characters."
            },
            user_email: "Valid email address required.",
            cpassword:{
                required: "Please retype your password",
                equalTo: "Passwords do no match!"
            }
        },
        submitHandler: submitForm
    });

    function submitForm() {
        var thisScope = this;

        var enc = '';

        $("#cpassword").keyup(function () {
            var value = $(this).val();
            enc = sha256_digest(value);
        }).keyup();

        $('#password').val(enc);
        $('#cpassword').val(enc);

        regFormData = $("#register-form").serialize();

        $.ajax({
            type : 'POST',
            url  : 'php/register.php',
            data : regFormData,
            beforeSend: function() {
                $("#error").fadeOut();
                $("#btn-submit")
                    .html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; Sending ...');
            },
            success :  function(data) {
                if(data==1) {
                    $("#error").fadeIn(1000, function() {
                        $("#error")
                            .html('<div class="alert alert-danger"> <span class="glyphicon glyphicon-info-sign"></span> &nbsp; Sorry, email address already taken!</div>');
                        $("#btn-submit")
                            .html('<span class="glyphicon glyphicon-log-in"></span> &nbsp; Create Account');
                    });
                }
                else if(data==2) {
                    $("#error").fadeIn(1000, function() {
                        $("#error")
                            .html('<div class="alert alert-danger"> <span class="glyphicon glyphicon-info-sign"></span> &nbsp; Sorry, user name already taken!</div>');
                        $("#btn-submit")
                            .html('<span class="glyphicon glyphicon-log-in"></span> &nbsp; Create Account');
                    });
                }
                else if(data=="registered") {

                    $("#btn-submit").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; Signing In ...');
                    setTimeout(angularRedirect(), 4000);
                }
                else {
                    $("#error").fadeIn(1000, function() {
                        $("#error")
                            .html('<div class="alert alert-danger"><span class="glyphicon glyphicon-info-sign"></span> &nbsp; '+data+'!</div>');
                        $("#btn-submit")
                            .html('<span class="glyphicon glyphicon-log-in"></span> &nbsp; Create Account');
                    });
                }
            }
        });
        return false;
    }

    function angularRedirect() {
        // Create sql table to hold user recipes
        $.ajax({
            type: 'post',
            url: 'php/createRecipeTable.php',
            data: {
                table_name: $('#user_name').val()
            },
            success: function( data ) {
                console.log( data );
            }
        });

        $.ajax({

            type : 'POST',
            url  : 'php/login.php',
            data : regFormData,
            beforeSend: function()
            {
                $("#error").fadeOut();
                $("#btn-login").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; sending ...');
            },
            success :  function(response)
            {
                var now = Date.now();
                var expires = now + 3600000;
                //console.log('Authentication expires in: ' + ((expires - now)/60)/1000 + ' minutes');
                localStorageService.set('auth', true);
                localStorageService.set('expires', expires);
                $location.path('/dashboard');
                $scope.$apply();
            }
        });
    }
});

mrPour.controller('loginController', function ($scope, $rootScope, $location, localStorageService) {

    if (localStorageService.get('auth')) {
        $location.path('/dashboard');
        $scope.$apply();
    }

    $("#login-form").validate({
        rules:
        {
            password: {
                required: true
            },
            user_email: {
                required: true,
                email: true
            }
        },
        messages:
        {
            password:{
                required: "please enter your password"
            },
            user_email: "please enter your email address"
        },
        submitHandler: submitForm
    });
    /* validation */

    /* login submit */
    function submitForm() {

        var enc = '';

        $("#password").keyup(function () {
            var value = $(this).val();
            enc = sha256_digest(value);
        }).keyup();

        $('#password').val(enc);

        var data = $("#login-form").serialize();

        $.ajax({

            type : 'POST',
            url  : 'php/login.php',
            data : data,
            beforeSend: function()
            {
                $("#error").fadeOut();
                $("#btn-login").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; sending ...');
            },
            success :  function(response)
            {
                if(response=="ok"){

                    $("#btn-login").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; Signing In ...');
                    setTimeout(angularRedirect(), 4000);
                }
                else{
                    $("#error").fadeIn(1000, function(){
                        $("#error").html('<div class="alert alert-danger"> <span class="glyphicon glyphicon-info-sign"></span> &nbsp; '+response+' !</div>');
                        $("#btn-login").html('<span class="glyphicon glyphicon-log-in"></span> &nbsp; Sign In');
                    });
                }
            }
        });
        return false;
    }

    function angularRedirect () {
        var now = Date.now();
        var expires = now + 3600000;
        //console.log('Authentication expires in: ' + ((expires - now)/60)/1000 + ' minutes');
        localStorageService.set('auth', true);
        localStorageService.set('expires', expires);
        $location.path('/dashboard');
        $scope.$apply();
    }

});

mrPour.controller('dashboardController', function ($scope, $rootScope, $http, localStorageService, $location, $anchorScroll) {

    $scope.modalShown = false;

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response) {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $scope.scrollTo = function(id) {
        $location.hash(id);
        $anchorScroll();
    };

});

mrPour.controller('recipeController', function ($scope, $rootScope, $http, localStorageService, $location, $timeout) {

    $scope.modalShown = false;

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response)
            {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $('#updateRecipe').hide();
    $('#deleteRecipe').hide();
    $('#pourRecipe').hide();

    $scope._userRecipes = '';

    var gridDiv = document.querySelector('#myGrid');

    var columnDefs = [
        { headerName: 'ID', field: 'id', width: 50, editable: false, suppressSizeToFit: true },
        { headerName: 'Recipe Name', field: 'recipe_name', width: 200, editable: false },
        { headerName: 'Ingredient 1', field: 'ingredient_1', width: 200, editable: false },
        { headerName: 'Amount 1', field: 'amount_1', width: 150, editable: false },
        { headerName: 'Ingredient 2', field: 'ingredient_2', width: 200, editable: false },
        { headerName: 'Amount 2', field: 'amount_2', width: 150, editable: false },
        { headerName: 'Ingredient 3', field: 'ingredient_3', width: 200, editable: false },
        { headerName: 'Amount 3', field: 'amount_3', width: 150, editable: false },
        { headerName: 'Ingredient 4', field: 'ingredient_4', width: 200, editable: false },
        { headerName: 'Amount 4', field: 'amount_4', width: 150, editable: false },
        { headerName: 'Glass Type', field: 'glass_type', editable: false }
    ];

    var gridOptions = {
        columnDefs: columnDefs,
        enableSorting: true,
        enableColResize: true,
        rowDeselection: true,
        rowSelection: 'single',
        onRowSelected: rowSelectedFunc,
        onRowDeselected: rowDeslectedFunc
    };

    new ag.grid.Grid(gridDiv, gridOptions);

    function rowSelectedFunc (event) {
        $('#updateRecipe').show();
        $('#deleteRecipe').show();
       	$('#pourRecipe').show();
	 $scope.rowData = event.node.data;
    }

    function rowDeslectedFunc (event) {
        $('#updateRecipe').hide();
        $('#deleteRecipe').hide();
	    $('#pourRecipe').hide();
        $scope.rowData = event.node.data;
    }

    $scope.createNewRecipe = function() {
        $location.path('/dashboard/recipes/create');
    };

    $scope.updateRecipe = function() {
        $rootScope.recipeData = $scope.rowData;
        $location.path('/dashboard/recipes/update');
    };

    $scope.pourRecipe = function() {
        while($rootScope.isBusy);
        $rootScope.isBusy = true;

        $scope.rowData['control'] = 'p';
        $.ajax({
            type: 'post',
            url: 'php/sendIngredientAmounts.php',
            data: $scope.rowData,
            beforeSend: function () {
                $scope.modalShown = true;
            },
            success: function () {
                // Show pouring dialog until finished
                var totalAmount = parseFloat($scope.rowData['amount_1'])
                                    + parseFloat($scope.rowData['amount_2'])
                                    + parseFloat($scope.rowData['amount_3'])
                                    + parseFloat($scope.rowData['amount_4']);
                var pouringTime = totalAmount * 26000;  // Guess/check with little buffer
                setInterval(function() {
                    $scope.modalShown = false;
                    $rootScope.isBusy = false;
                    $scope.$apply();
                }, pouringTime);
            }
        });
    };

    $scope.deleteRecipe = function() {
        $scope.rowData['table_name'] = $rootScope.userData.user_name;
        // Delete selected recipe from table
        $.ajax({
            type: 'post',
            url: 'php/deleteRecipe.php',
            data: $scope.rowData,
            beforeSend: function() {

            },
            success: function ( data ) {
                $timeout(location.reload(true), 1000);
            }
        });
    };

    $.getJSON('php/getRecipes.php', function(data) {
        $scope.$apply(function() {
            $scope.hasRecipes = true;
            $scope._userRecipes = data;
            gridOptions.api.setRowData(data);
        });
    }).error(function() { console.log('No recipes exist!'); $scope.hasRecipes = false; });
});

mrPour.controller('createController', function ($scope, $rootScope, $http, localStorageService, $location) {

    $scope.modalShown = false;

    $scope.logout = function() {
        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response)
            {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $scope.recipe = {};


    $scope.cancel = function() {
        $location.path('/dashboard/recipes');
    };

    $scope.saveRecipe = function(recipe) {
        recipe['table_name'] = $rootScope.userData.user_name;
        // Create sql table to hold user recipes
        $.ajax({
            type: 'post',
            url: 'php/saveRecipe.php',
            data: recipe,
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success: function ( data ) {
                setInterval(function() {
                    $scope.modalShown = false;
                }, 1000);
                $location.path('/dashboard/recipes');
                $scope.$apply();
            }
        });
    }
});

mrPour.controller('updateController', function ($scope, $rootScope, $http, localStorageService, $location) {

    $scope.modalShown = false;

    if (!$rootScope.recipeData) {
        $location.path('/dashboard/recipes');
    }

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response)
            {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $scope.recipe = $rootScope.recipeData;

    $scope.recipe.amount_1 = Number($scope.recipe.amount_1);
    $scope.recipe.amount_2 = Number($scope.recipe.amount_2);
    $scope.recipe.amount_3 = Number($scope.recipe.amount_3);
    $scope.recipe.amount_4 = Number($scope.recipe.amount_4);

    $scope.cancel = function() {
        $location.path('/dashboard/recipes');
    };

    $scope.saveRecipe = function(recipe) {

        recipe['table_name'] = $rootScope.userData.user_name;
        // Create sql table to hold user recipes
        $.ajax({
            type: 'post',
            url: 'php/updateRecipe.php',
            data: recipe,
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success: function ( data ) {
                setInterval(function() {
                    $scope.modalShown = false;
                }, 1000);
                console.log(data);
            }
        });
    }
});

mrPour.controller('tempController', function ($scope, $rootScope, $http, localStorageService, $location) {

    $scope.modalShown = false;

    $scope.val = {};
    $scope.val['control'] = 't';

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response) {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $scope.currentlySetTemp = localStorageService.get('currentlySetTemp');
    if ($scope.currentlySetTemp < 35 && $scope.currentlySetTemp > 55) {
        $scope.currentlySetTemp = 35;
        localStorageService.set('currentlySetTemp', $scope.currentlySetTemp);
        $scope.sendUpdatedTemp();
    }

    $('#currentlySetTemp').text($scope.currentlySetTemp);
    $('#currentTempSpan').text($rootScope.temp['temp']);

    // Randomly add a data point every 500ms
    var random = new TimeSeries();
    setInterval(function() {
        random.append(new Date().getTime(), $rootScope.temp['temp']);
        $('#currentTempSpan').text($rootScope.temp['temp']);
    }, 500);

    $scope.decreaseTemp = function() {
        if ($scope.currentlySetTemp > 35) {
            while($rootScope.isBusy);
            $scope.modalShown = true;
            $rootScope.isBusy = true;
            $scope.currentlySetTemp--;
            $scope.sendUpdatedTemp();
        }
    };

    $scope.increaseTemp = function() {
        if ($scope.currentlySetTemp < 55) {
            while($rootScope.isBusy);
            $scope.modalShown = true;
            $rootScope.isBusy = true;
            $scope.currentlySetTemp++;
            $scope.sendUpdatedTemp();
        }
    };

    $scope.sendUpdatedTemp = function() {
        $('#currentlySetTemp').text($scope.currentlySetTemp);
        localStorageService.set('currentlySetTemp', $scope.currentlySetTemp);
        $scope.val['status'] = $scope.currentlySetTemp;
        // Send updated temp to the avr
        $.ajax({
            type: 'post',
            url: 'php/updateCoolingStatus.php',
            data: $scope.val,
            success: function ( data ) {
                setInterval(function() {
                    $rootScope.isBusy = false;
                    $scope.modalShown = false;
                    $scope.$apply();
                }, 1500);
            }
        });
    };

    var chart = new SmoothieChart({
	horizontalLines: [
		{
			color:'#B20000',
			lineWidth:2,
			value:55
		},
		{	
			color:'#59C3D8',
			lineWidth:2,
			value:35
		}
	],
	minValue: 20,
        maxValue: 70,
        millisPerPixel: 100,
        grid: {
            fillStyle: '#eeeeee',
            millisPerLine: 10000,
            verticalSections: 12
        },
        labels: {
            fillStyle: '#000000',
            fontSize: 15
        },
        timestampFormatter:SmoothieChart.timeFormatter
    }),
    canvas = document.getElementById('smoothie-chart'),
    series = new TimeSeries();

    chart.addTimeSeries(random, {
        lineWidth: 2.0,
        strokeStyle: '#000000'
    });

    chart.streamTo(canvas, 500);
});
