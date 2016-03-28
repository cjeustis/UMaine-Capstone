var mrPour = angular.module('mrPour', ['ngAnimate', 'ngRoute', 'LocalStorageModule', 'ui.bootstrap', 'agGrid', 'ngMessages']);

mrPour.config(function ($routeProvider, localStorageServiceProvider) {

    localStorageServiceProvider
        .setPrefix('mrPour')
        .setStorageType('localStorage')
        .setNotify(true, true);


    $routeProvider
        .when('/index.html', {
            templateUrl: 'main/main.html',
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
            templateUrl: 'registration/registration-page.html',
            controller: 'registrationController'
        })

        .when('/login', {
            templateUrl: 'login/login-page.html',
            controller: 'loginController'
        })

        .when('/dashboard', {
            templateUrl: 'dashboard/dash.html',
            controller: 'dashboardController'
        })

        .when('/dashboard/recipes', {
            templateUrl: 'recipe/recipes.html',
            controller: 'recipeController'
        })

        .when('/dashboard/recipes/create', {
            templateUrl: 'create-recipe/createRecipe.html',
            controller: 'createController'
        })

        .when('/dashboard/recipes/update', {
            templateUrl: 'update-recipe/updateRecipe.html',
            controller: 'updateController'
        })

        .when('/dashboard/temperature', {
            templateUrl: 'temperature/temp.html',
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
                    $rootScope.temp['temp'] = ((d.Temp * 9) / 5 + 32) + 3.5;
                    if ($rootScope.temp['temp'] < 45) {
                        $rootScope.temp['temp'] += 1;
                    }
                    if ($rootScope.temp['temp'] < 40) {
                        $rootScope.temp['temp'] += 1;
                    }
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
                        if ($rootScope.temp['temp'] < 45) {
                            $rootScope.temp['temp'] += 1;
                        }
                        if ($rootScope.temp['temp'] < 40) {
                            $rootScope.temp['temp'] += 1;
                        }
                    }
                });
            }

            // send temp value to avr
            $.ajax({
                type: 'post',
                url: 'php/sendTempData.php',
                data: $rootScope.temp,
                success: function (data) {
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