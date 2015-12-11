var mrPour = angular.module('mrPour', ['ngRoute']);

mrPour.config(function ($routeProvider) {
    $routeProvider

    .when('/', {
        templateUrl: 'pages/home.html',
        controller: 'mainController'
    })

    .when('/about', {
        templateUrl: 'pages/about.html',
        controller: 'aboutController'
    })

    .when('/contact', {
        templateUrl: 'pages/contact.html',
        controller: 'contactController'
    });
});

// create the controller and inject Angular's $scope
mrPour.controller('mainController', function ($scope) {
    // create a message to display in our view
    $scope.message = 'This is the home page!';
});

mrPour.controller('aboutController', function ($scope) {
    $scope.message = 'This is the about page!';
});

mrPour.controller('contactController', function ($scope) {
    $scope.message = 'This is the contact page!'
})