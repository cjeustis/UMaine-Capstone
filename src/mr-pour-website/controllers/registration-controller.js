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